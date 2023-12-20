#![feature(let_chains)]

use smart_home::devices::socket::Socket;
use smart_home::devices::thermo::Thermo;
use smart_home::devices::visitors::{ReportVisitor, SwitchStatusVisitor};
use smart_home::devices::Device;
use smart_home::house::house::House;
use std::ops::{Deref, DerefMut};
use std::sync::{Arc, RwLock};
use std::thread::sleep;
use std::time::Duration;
use std::{
    io::prelude::*,
    net::{TcpListener, TcpStream},
};
use tera::Tera;

type HouseImpl = House<Device>;

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

async fn handle_connection(mut stream: TcpStream, house: Arc<RwLock<HouseImpl>>) {
    println!("handling new connection");
    sleep(Duration::from_secs(2));

    let uri = parse_uri(&mut stream);
    if let Some(uri) = uri
        && uri.starts_with("/switch_status")
    {
        let mut lock = house.write().unwrap();
        handle_switch(&uri, &mut stream, lock.deref_mut());
        return;
    }
    let lock = house.read().unwrap();
    handle_default(&mut stream, lock.deref())
}

fn handle_switch(uri: &str, stream: &mut TcpStream, house: &mut HouseImpl) {
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

fn handle_default(stream: &mut TcpStream, house: &HouseImpl) {
    let mut reporter = ReportVisitor::default();
    _ = house.visit_devices(&mut reporter, None);

    let mut tera_ctx = tera::Context::new();

    tera_ctx.insert("reports", &reporter.reports);

    let device_types = vec!["socket", "thermometer"];
    tera_ctx.insert("device_types", &device_types);
    // println!("{:?}", http_request);

    let tpl_name = "index.html";
    let tpl_data = include_str!("response_tmpl/index.html");
    let mut tera = Tera::default();
    tera.add_raw_template(tpl_name, tpl_data).unwrap();

    let html = match tera.render(tpl_name, &tera_ctx) {
        Ok(t) => t,
        Err(e) => format!("Render error: {:?}", e),
    };
    let response = format!("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n{html}");
    stream.write_all(response.as_bytes()).unwrap();
}

#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {
    let mut house = HouseImpl::new();
    let bathroom = "bathroom";

    house.add_room(bathroom)?;
    if let Err(e) = house.add_room(bathroom) {
        println!("handler error: {}", e);
    }
    let mut reportable_thermo = Thermo::new("bath_termo1".to_string());
    reportable_thermo.enable_udp_updates(4444, 5555);
    house.add_device(bathroom, reportable_thermo.into())?;
    house.add_device(bathroom, Socket::new("bath_socket1".to_string()).into())?;

    let living = "living_room";
    house.add_room(living)?;
    house.add_device(
        living,
        Thermo::new("living_termo_window".to_string()).into(),
    )?;
    house.add_device(living, Thermo::new("living_termo_door".to_string()).into())?;

    let house = Arc::new(RwLock::new(house));
    let listener = TcpListener::bind("127.0.0.1:8080")?;
    println!("listening on http://127.0.0.1:8080");

    for stream in listener.incoming() {
        println!("new connection");
        let house_th = house.clone();
        let stream = stream.unwrap();

        tokio::spawn(async move {
            handle_connection(stream, house_th).await;
        });
    }
    Ok(())
}
