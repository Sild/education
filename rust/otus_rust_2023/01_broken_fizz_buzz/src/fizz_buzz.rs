#![feature(test)]
use std::vec::Vec;
use criterion::{black_box, criterion_group, criterion_main, Criterion};

extern crate test;

pub fn naive(dst: &mut Vec<String>) {
    for i in 1..=dst.len() {
        let s = match (i % 3, i % 5) {
            (0, 0) => String::from("FizzBuzz"),
            (0, _) => String::from("Fizz"),
            (_, 0) => String::from("Buzz"),
            (_, _) => format!("{}", i),
        };
        dst[i - 1] = s;
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use test::Bencher;

    fn get_dst_store(dst_size: usize) -> Vec<String> {
        let mut result = Vec::new();
        result.resize(dst_size, String::from(""));
        return result;
    }

    fn get_canonical(dst_size: usize) -> Vec<String> {
        let mut dst = get_dst_store(dst_size);
        naive(&mut dst);
        return dst;
    }

    #[test]
    fn naive_test() {
        let dst_size = 100;
        let expected = get_canonical(dst_size);
        let mut given = get_dst_store(dst_size);
        naive(&mut given);
        assert_eq!(expected, given);
    }

    #[bench]
    fn naive_perf(b: &mut Bencher) {
        let mut dst = get_dst_store(100);
        b.iter(|| naive(&mut dst));
    }
}
