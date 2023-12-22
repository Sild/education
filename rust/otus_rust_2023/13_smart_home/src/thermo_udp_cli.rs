use std::io::Write;
use std::{io, net::UdpSocket};

fn main() {
    let socket = UdpSocket::bind("127.0.0.1:0").unwrap();
    let mut input = String::new();
    loop {
        print!("enter new temp: ");
        _ = io::stdout().flush();
        input.clear();
        io::stdin().read_line(&mut input).unwrap();
        socket.send_to(input.as_bytes(), "127.0.0.1:4444").unwrap();
    }
}
