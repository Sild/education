use std::vec::Vec;

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

pub fn naive_unwrap(dst: &mut Vec<String>) {
    let mut i = 1;

    while i <= dst.len() {
        let s = match (i % 3, i % 5) {
            (0, 0) => String::from("FizzBuzz"),
            (0, _) => String::from("Fizz"),
            (_, 0) => String::from("Buzz"),
            (_, _) => format!("{}", i),
        };
        i += 1;
        if i == dst.len() {
            break;
        }
        dst[i - 1] = s;
        let s = match (i % 3, i % 5) {
            (0, 0) => String::from("FizzBuzz"),
            (0, _) => String::from("Fizz"),
            (_, 0) => String::from("Buzz"),
            (_, _) => format!("{}", i),
        };
        dst[i - 1] = s;
        i += 1;
    }
}

pub fn naive_print(max_num: usize) {
    for i in 1..=max_num {
        let _s = match (i % 3, i % 5) {
            (0, 0) => String::from("FizzBuzz"),
            (0, _) => String::from("Fizz"),
            (_, 0) => String::from("Buzz"),
            (_, _) => format!("{}", i),
        };
    }
}

pub fn naive_print2(max_num: usize) {
    for i in 1..=max_num {
        let mut s: String;
        if i % 3 == 0 {
            if i % 5 == 0 {
                s = String::from("FizzBuzz")
            } else {
                s = String::from("Fizz")
            }
        } else if i % 5 == 0 {
            s = String::from("Buzz")
        } else {
            s = format!("{}", i)
        }
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
        let mut dst = get_dst_store(1000);
        b.iter(|| naive(&mut dst));
    }

    #[bench]
    fn naive_unwrap_perf(b: &mut Bencher) {
        let mut dst = get_dst_store(1000);
        b.iter(|| naive_unwrap(&mut dst));
    }

    #[bench]
    fn naive_print_perf(b: &mut Bencher) {
        b.iter(|| naive_print(1000));
    }
    #[bench]
    fn naive_print2_perf(b: &mut Bencher) {
        b.iter(|| naive_print2(1000));
    }
}
