use smart_home::devices::socket::Socket;
use smart_home::devices::thermo::Thermometer;
use smart_home::devices::visitors::{ReportVisitor, TurnOnVisitor};
use smart_home::house::house::House;
use std::io::Error;
use std::{
    io::{prelude::*, BufReader},
    net::{TcpListener, TcpStream},
};
use itertools::Itertools;
// use serde::{Deserialize, Serialize};
use tera::Tera;

fn print_device_report(house: &House, report_tag: &str) {
    println!("\n{}", report_tag);
    let mut reporter = ReportVisitor::default();
    _ = house.visit_devices(&mut reporter, None);
    reporter.print_report();
}

fn handle_connection(mut stream: TcpStream, house: &mut House) {
    let buf_reader = BufReader::new(&mut stream);
    let http_request: Vec<_> = buf_reader
        .lines()
        .map(|result| result.unwrap())
        .take_while(|line| !line.is_empty())
        .collect();

    let mut reporter = ReportVisitor::default();
    _ = house.visit_devices(&mut reporter, None);

    let mut teraCtx = tera::Context::new();

    let mut rooms = reporter.reports.iter().map(|x| {x.0}).collect_vec();
    rooms.sort();
    teraCtx.insert("rooms", &rooms);


    let tpl_name = "index.html";
    let tpl_data = include_str!("html_tpl/index.html");
    let mut tera = Tera::default();
    tera.add_raw_template(tpl_name, tpl_data).unwrap();


    let html = tera.render(tpl_name, &teraCtx).unwrap();
    let response = format!("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n{html}");


    stream.write_all(response.as_bytes()).unwrap();
}

fn main() -> Result<(), Error> {
    let mut house = House::default();
    let bathroom = "bathroom";

    house.add_room(bathroom)?;
    house.add_device(bathroom, Thermometer::new("bath_termo1".to_string()))?;
    house.add_device(bathroom, Socket::new("bath_socket1".to_string()))?;

    let living = "living_room";
    house.add_room(living)?;
    house.add_device(living, Thermometer::new("living_termo_window".to_string()))?;
    house.add_device(living, Thermometer::new("living_termo_door".to_string()))?;

    let listener = TcpListener::bind("127.0.0.1:8080").unwrap();

    for stream in listener.incoming() {
        let stream = stream.unwrap();
        handle_connection(stream, &mut house);
    }

    print_device_report(&house, "===default report===");

    let mut turn_on = TurnOnVisitor::default();
    _ = house.visit_devices_mut(&mut turn_on, Some(bathroom));

    print_device_report(&house, "===turn_on report===");

    _ = house.extract_device::<Socket>(bathroom, "bath_socket1");

    print_device_report(&house, "===extract report===");

    Ok(())
}


