extern crate rand;

use std::io;
use std::cmp::Ordering;
use rand::Rng;

fn main() {
    println!("Guess!");
    
    loop {
        let secret = rand::thread_rng().gen_range(1, 6);
        println!("secret is {}", secret);

        print!("Enter num: ");
        let mut guess = String::new();
        io::stdin().read_line(&mut guess).expect("Failed to read line");
        let guess: u32 = match guess.trim().parse() {
            Ok(num) => num,
            Err(_) => continue,
        };

        match guess.cmp(&secret) {
            Ordering::Less => println!("small"),
            Ordering::Greater => println!("greater"),
            Ordering::Equal => {
                println!("eq");
                break;
            },
        }
    }

}
