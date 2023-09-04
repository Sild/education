#![feature(test)]
extern crate test;

mod fizz_buzz;
use std::vec::Vec;

fn main() {
    // const COUNT: usize = 100;
    // let mut result = Vec::new();
    // result.resize(COUNT, String::from(""));
    // fizz_buzz::naive(&mut result);
    // print_result(&result);

    fizz_buzz::naive_print(1000);
    fizz_buzz::naive_print2(1000);
}

fn print_result(result: &Vec<String>) {
    for r in result {
        println!("{}", r)
    }
}
