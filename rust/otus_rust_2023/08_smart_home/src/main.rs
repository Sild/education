#![feature(let_chains)]

use smart_home::devices::socket::Socket;
use smart_home::devices::thermo::Thermo;
use smart_home::devices::visitors::{ReportVisitor, SwitchStatusVisitor};
use smart_home::house::house::House;
use std::io::Error;
use std::{
    io::prelude::*,
    net::{TcpListener, TcpStream},
};
use tera::Tera;

fn parse_uri(stream: &mut TcpStream) -> Option<String> {
    let mut buffer = [0; 1024];
    let n = stream
        .read(&mut buffer)
        .expect("failed to read from stream");

    let request = std::str::from_utf8(&buffer[..n]).ok()?;
    let lines: Vec<&str> = request.lines().collect();
    if lines.is_empty() {
        return None;
    }
    let first_line = lines[0];
    let parts: Vec<&str> = first_line.split_whitespace().collect();
    if parts.len() < 2 {
        return None;
    }

    Some(parts[1].to_string())
}

fn handle_connection(mut stream: TcpStream, house: &mut House) {
    let uri = parse_uri(&mut stream);
    if let Some(uri) = uri && uri.starts_with("/switch_status") {
        handle_switch(&uri, &mut stream, house);
        return;
    }
    handle_default(&mut stream, house)
}

fn handle_switch(uri: &str, stream: &mut TcpStream, house: &mut House) {
    let args = uri.split('/').collect::<Vec<_>>();
    let room = args.get(2);
    let device = args.get(3);
    if room.is_none() || device.is_none() {
        return;
    }
    let mut visitor = SwitchStatusVisitor::new(vec![*device.unwrap()]);
    match house.visit_devices_mut(&mut visitor, Some(room.unwrap())) {
        Ok(_) => {}
        Err(e) => {
            println!("error: {:?}", e)
        }
    }
    let response = "HTTP/1.1 302 OK\r\nLocation: /\r\n\r\n".to_string();
    stream.write_all(response.as_bytes()).unwrap();
}

fn handle_default(stream: &mut TcpStream, house: &House) {
    let mut reporter = ReportVisitor::default();
    _ = house.visit_devices(&mut reporter, None);

    let mut tera_ctx = tera::Context::new();

    tera_ctx.insert("reports", &reporter.reports);
    // println!("{:?}", http_request);

    let tpl_name = "index.html";
    let tpl_data = include_str!("html_tpl/index.html");
    let mut tera = Tera::default();
    tera.add_raw_template(tpl_name, tpl_data).unwrap();

    let html = match tera.render(tpl_name, &tera_ctx) {
        Ok(t) => t,
        Err(e) => format!("Render error: {:?}", e),
    };
    let response = format!("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n{html}");
    stream.write_all(response.as_bytes()).unwrap();
}

fn main() -> Result<(), Error> {
    let mut house = House::default();
    let bathroom = "bathroom";

    house.add_room(bathroom)?;
    let mut reportable_thermo = Thermo::new("bath_termo1".to_string());
    reportable_thermo.enable_udp_updates(4444, 5555);
    house.add_device(bathroom, reportable_thermo)?;
    house.add_device(bathroom, Socket::new("bath_socket1".to_string()))?;

    let living = "living_room";
    house.add_room(living)?;
    house.add_device(living, Thermo::new("living_termo_window".to_string()))?;
    house.add_device(living, Thermo::new("living_termo_door".to_string()))?;

    let listener = TcpListener::bind("127.0.0.1:8080").unwrap();

    for stream in listener.incoming() {
        let stream = stream.unwrap();
        handle_connection(stream, &mut house);
    }

    Ok(())
}
