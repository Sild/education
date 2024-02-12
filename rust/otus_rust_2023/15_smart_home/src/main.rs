use std::io::{stdin, stdout, Write};

extern "C" {
    pub fn switch_socket_status();
    pub fn print_socket_report();
}

fn switch_socket_status_safe() {
    unsafe {
        switch_socket_status();
    }
}

fn print_socket_report_safe() {
    unsafe {
        print_socket_report();
    }
}

fn main() {
    println!("Socket cli started!");
    let mut user_input = String::new();

    loop {
        print!("s - switch status, r - get report, q - quit. Your input: ");
        let _ = stdout().flush();
        user_input.clear();
        stdin()
            .read_line(&mut user_input)
            .expect("Did not enter a correct string");
        match user_input {
            ref ui if ui.trim() == "s" => {
                switch_socket_status_safe();
                println!("Success!");
            }
            ref ui if ui.trim() == "r" => {
                print_socket_report_safe();
            }
            ref ui if ui.trim() == "q" => {
                println!("Goodbye!");
                break;
            }
            _ => {
                println!("Unknown input!");
            }
        }
    }
}
