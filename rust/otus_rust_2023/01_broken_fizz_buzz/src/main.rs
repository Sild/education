mod fizz_buzz;
use std::vec::Vec;

fn main() {
    const COUNT: usize = 100;
    let mut result = Vec::new();
    result.resize(COUNT, String::from(""));
    fizz_buzz::naive(&mut result);
    print_result(&result);
}

fn print_result(result: &Vec<String>) {
    for r in result {
        println!("{}", r)
    }
}
