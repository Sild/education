#pragma once
#include <xmmintrin.h>
#include <immintrin.h>
#include "helpers.h"

namespace {

// CPUs support RAM access like this: "ymmword ptr [rax+64]"
// Using templates with offset int argument to make easier for compiler to emit good code.
// Returns acc + ( p1 * p2 ), for 8 float lanes
template<int offsetRegs, typename DataT>
inline auto fma8( __m512 acc, const DataT* p1, const DataT* p2 )
{
    constexpr ptrdiff_t lanes = offsetRegs * 16;
    const auto a = _mm512_loadu_ps( p1 + lanes );
    const auto b = _mm512_loadu_ps( p2 + lanes );
    return _mm512_fmadd_ps( a, b, acc );
}

#ifdef __AVX512F__
inline __m512i makeRemainderMask( ptrdiff_t missingLanes )
{
    // Make a mask of 8 bytes
    // These aren't branches, they should compile to conditional moves
    missingLanes = std::max( missingLanes, (ptrdiff_t)0 );
    uint64_t mask = -( missingLanes < 16 );
    mask >>= missingLanes * 16;
    // Sign extend the bytes into int32 lanes in AVX vector
    __m256i tmp =  _mm_cvti64_sd( (int64_t)mask );
    return _mm512_cvtepi8_epi32( tmp );
}
#else
// Aligned by 64 bytes
// The load will only touch a single cache line, no penalty for unaligned load
static const int alignas( 64 ) s_remainderLoadMask[ 16 ] = {
    -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0 };
inline __m256i makeRemainderMask( ptrdiff_t missingLanes )
{
    // These aren't branches, they compile to conditional moves
    missingLanes = std::max( missingLanes, (ptrdiff_t)0 );
    missingLanes = std::min( missingLanes, (ptrdiff_t)8 );
    // Unaligned load from a constant array
    const int* rsi = &s_remainderLoadMask[ missingLanes ];
    return _mm256_loadu_si256( ( const __m256i* )rsi );
}
#endif

// Same as fma8(), load conditionally using the mask
// When the mask has all bits set, an equivalent of fma8(), but 1 instruction longer
// When the mask is a zero vector, the function won't load anything, will return `acc`
template<int offsetRegs, typename DataT>
inline auto fma8rem( __m512 acc, const DataT* p1, const DataT* p2, ptrdiff_t rem )
{
    constexpr ptrdiff_t lanes = offsetRegs * 16;
    // Generate the mask for conditional loads
    // The implementation depends on whether AVX2 is enabled with compiler switches
    const __m512i mask = makeRemainderMask( ( 16 + lanes ) - rem );
    // These conditional load instructions produce zeros for the masked out lanes
    // constexpr if (std::is_same_v<DataT, float>) {
    const __m512i a = _mm512_maskload_ps( p1 + lanes, mask );
    const __m512i b = _mm512_maskload_ps( p2 + lanes, mask );
    return _mm512_fmadd_ps( a, b, acc );
    // }
    // } else {
    //     const __m256 a = _mm256_maskload_ps( p1 + lanes, mask );
    //     const __m256 b = _mm256_maskload_ps( p2 + lanes, mask );
    //     return _mm256_fmadd_ps( a, b, acc );
    // }

}

// Compute dot product of float vectors, using 8-wide FMA instructions
template<typename DataT>
float dotProductFma( const std::vector<DataT>& a, const std::vector<DataT>& b )
{
    assert( a.size() == b.size() );
    const size_t length = a.size();
    if( length == 0 )
        return DataT(0.0);

    const DataT* p1 = a.data();
    const DataT* p2 = b.data();
    // Compute length of the remainder; 
    // We want a remainder of length [ 1 .. 32 ] instead of [ 0 .. 31 ]
    const ptrdiff_t rem = ( ( length - 1 ) % 64 ) + 1;
    const DataT* const p1End = p1 + length - rem;

    // Initialize accumulators with zeros
    auto dot0 = _mm512_setzero_ps();
    auto dot1 = _mm512_setzero_ps();
    auto dot2 = _mm512_setzero_ps();
    auto dot3 = _mm512_setzero_ps();

    // Process the majority of the data.
    // The code uses FMA instructions to multiply + accumulate, consuming 32 values per loop iteration.
    // Unrolling manually for 2 reasons:
    // 1. To reduce data dependencies. With a single register, every loop iteration would depend on the previous result.
    // 2. Unrolled code checks for exit condition 4x less often, therefore more CPU cycles spent computing useful stuff.
    while( p1 < p1End )
    {
        dot0 = fma8<0>( dot0, p1, p2 );
        dot1 = fma8<1>( dot1, p1, p2 );
        dot2 = fma8<2>( dot2, p1, p2 );
        dot3 = fma8<3>( dot3, p1, p2 );
        p1 += 64;
        p2 += 64;
    }

    // Handle the last, possibly incomplete batch of length [ 1 .. 32 ]
    // To save multiple branches, we load that entire batch with `vmaskmovps` conditional loads
    // On modern CPUs, the performance of such loads is pretty close to normal full vector loads
    if (rem != 64) {
        // std::cout << "rem: " << rem << std::endl;
        dot0 = fma8rem<0>( dot0, p1, p2, rem );
        dot1 = fma8rem<1>( dot1, p1, p2, rem );
        dot2 = fma8rem<2>( dot2, p1, p2, rem );
        dot3 = fma8rem<3>( dot3, p1, p2, rem );
    }


    // Add 32 values into 8
    dot0 = _mm512_add_ps( dot0, dot2 );
    dot1 = _mm512_add_ps( dot1, dot3 );
    dot0 = _mm512_add_ps( dot0, dot1 );

    __m256 r8 = _mm_add_ss(_mm512_castps512_ps256(dot0), _mm512_castps512_ps256(dot0, 1));

    // Add 8 values into 4
    __m128 r4 = _mm_add_ps( _mm256_castps256_ps128( r8 ),
        _mm256_extractf128_ps( r8, 1 ) );
    // Add 4 values into 2
    r4 = _mm_add_ps( r4, _mm_movehl_ps( r4, r4 ) );
    // Add 2 lower values into the scalar result
    r4 = _mm_add_ss( r4, _mm_movehdup_ps( r4 ) );

    // Return the lowest lane of the result vector.
    // The intrinsic below compiles into noop, modern compilers return floats in the lowest lane of xmm0 register.
    return _mm_cvtss_f32( r4 );
}

}
// https://stackoverflow.com/questions/59494745/avx2-computing-dot-product-of-512-float-arrays
void registers512_float(const matrix& left, const matrix& right, matrix& result)
{
    for (int i = 0; i < MATRIX_SIZE; ++i)
    {
        for (int j = 0; j < MATRIX_SIZE; ++j)
        {
            result[i][j] += dotProductFma(left[i], right[j]);
            // result[i][j+1] += dotProductFma(left[i], right[j+1]);
            // result[i][j+2] += dotProductFma(left[i], right[j+2]);
            // result[i][j+3] += dotProductFma(left[i], right[j+3]);
            // j+=4;
            // for (int k = 0; k < MATRIX_SIZE; ++k)
            // {
            //     result[i][j] += left[i][k] * right[j][k];
            // }
        }
    }
}
