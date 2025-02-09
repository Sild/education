use criterion::{black_box, criterion_group, criterion_main, Criterion};

#[derive(Clone, Copy)]
struct DummyObject<const T: usize> {
    field2: [u8; T],
}

impl<const T: usize> DummyObject<T> {
    fn new() -> Self {
        DummyObject {
            field2: [1; T],
        }
    }
}

fn perf_ref<const T: usize>(obj: &DummyObject<T>) {
    let _val = obj.field2[obj.field2.len() - 1] ^ obj.field2[0];
}

fn perf_copy<const T: usize>(obj: DummyObject<T>) {
    let _val = obj.field2[obj.field2.len() - 1] ^ obj.field2[0];
}

macro_rules! run_bench {
    ($c:expr, $size:literal) => {
        let obj = DummyObject::<$size>::new();
        $c.bench_function(concat!("perf_ref_", $size), |b| {b.iter(|| perf_ref(black_box((&obj))));});
        $c.bench_function(concat!("perf_copy_", $size), |b| b.iter(|| perf_copy(black_box((obj)))));
    };
}


fn benchmark_functions(c: &mut Criterion) {
    run_bench!(c, 1);
    run_bench!(c, 2);
    run_bench!(c, 4);
    run_bench!(c, 8);
    run_bench!(c, 16);
    run_bench!(c, 32);
    run_bench!(c, 64);
    run_bench!(c, 128);
    run_bench!(c, 256);
    run_bench!(c, 512);
    run_bench!(c, 1024);
    run_bench!(c, 4096);
}

criterion_group!(benches, benchmark_functions);
criterion_main!(benches);
