use criterion::{black_box, criterion_group, criterion_main, Criterion};

#[derive(Clone)]
struct DummyObject<const T: usize> {
    #[allow(unused)]
    field2: [u8; T],
}

impl<const T: usize> DummyObject<T> {
    fn new() -> Self {
        DummyObject {
            field2: [1; T],
        }
    }
}

fn by_val<const T: usize>(obj: DummyObject<T>) -> DummyObject<T> {
    obj
}

fn by_ref<const T: usize>(obj: &DummyObject<T>) -> &DummyObject<T> {
    obj
}

macro_rules! run_bench {
    ($c:expr, $size:literal) => {
        let obj = DummyObject::<$size>::new();
        $c.bench_function(concat!("perf_ref_", $size), |b| b.iter(|| {
             let _copied = by_ref(black_box(&obj)); // Clone to ensure fresh copy
            black_box(_copied);
        }));
        $c.bench_function(concat!("perf_clone_", $size), |b| b.iter(|| {
            let _ref = by_val(black_box(obj.clone()));
            black_box(_ref);
        }));
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
    run_bench!(c, 40960);
}

criterion_group!(benches, benchmark_functions);
criterion_main!(benches);
