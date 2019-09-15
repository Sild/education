#pragma once

#include <iostream>
#include <utility>
#include <array>
#include <exception>

namespace NCustom {

template<typename T, size_t C>
struct TFixedAllocator: private std::allocator<T> {
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

    template<typename U>
    struct rebind {
        using other = TFixedAllocator<U, C>;
    };
    T *allocate(std::size_t n) {
        if(n > 1) { // too much pain for handle common cases
            auto p = alloc_impl(n);
            return reinterpret_cast<T*>(p);
        }
        if(HoldedCount == 0) {
            auto p = alloc_impl(C);
            ReservedBegin = reinterpret_cast<T*>(p);
            ReservedEnd = ReservedBegin + C;
            for(auto& hold: ReservedHold) {
                hold = false;
            }
        }
        for(size_t i = 0; i < C; ++i) {
            if(!ReservedHold[i]) {
                ReservedHold[i] = true;
                HoldedCount++; 
                return ReservedBegin + i;;
            }
        }
        auto p = alloc_impl(n);
        return reinterpret_cast<T*>(p);
    }

    void deallocate(T* const p, std::size_t n) {
        if( p > ReservedEnd || p < ReservedBegin) {
            std::free(p);
            return;
        }
        if(n > 1) {
            throw std::logic_error("Wrong branch");
        }
        auto tmp = ReservedBegin;
        for(auto& hold: ReservedHold) {
            if(p == tmp) {
                hold = false;
                if(--HoldedCount == 0) {
                    std::free(ReservedBegin);
                    ReservedBegin = nullptr;
                    ReservedEnd = nullptr;
                };
                return;
            } else {
                tmp++;
            }
        }
    }

    template<typename U, typename ...Args>
    void construct(U* p, Args&&...args) {
        new(p) U(std::forward<Args>(args)...);
    };

    void destroy(T *p) {
        p->~T();
    }

private:
    void* alloc_impl(size_t n) {
        auto p = std::malloc(n * sizeof(T));
        return p ? p : throw std::bad_alloc();
    }

    std::array<bool, C> ReservedHold;
    size_t HoldedCount = 0;
    T* ReservedBegin = nullptr;
    T* ReservedEnd = nullptr;
};
}