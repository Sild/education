#![feature(let_chains)]

use smart_home::devices::visitors::{ReportVisitor, SwitchStatusVisitor};
use smart_home::devices::Device;
use smart_home::house::house::House;
use std::collections::HashMap;
use std::sync::{Arc, RwLock};
use tera::Tera;
use tide::http::mime;

use smart_home::devices::socket::Socket;
use smart_home::devices::thermo::Thermo;
use tide::{Redirect, Request, Response, Result};

type HouseImpl = House<Device>;

fn build_house() -> Result<HouseImpl> {
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
    Ok(house)
}

fn handle_error(err: &str) -> tide::Result {
    let tpl_name = "error.html";
    let tpl_data = include_str!("response_tmpl/error.html");
    let mut tera = Tera::default();
    tera.add_raw_template(tpl_name, tpl_data).unwrap();
    let mut tera_ctx = tera::Context::new();
    tera_ctx.insert("error_text", &err.to_string());

    let html = match tera.render(tpl_name, &tera_ctx) {
        Ok(t) => t,
        Err(e) => format!("Render error: {:?}", e),
    };
    let response = Response::builder(400)
        .body(html.as_str())
        .content_type(mime::HTML)
        .build();
    Ok(response)
}

async fn handle_switch_status(req: Request<Arc<RwLock<HouseImpl>>>) -> tide::Result {
    let params: HashMap<String, String> = req.query()?;
    let room = match params.get("room_name") {
        Some(r) => r,
        None => return handle_error("argument room_name is required"),
    };
    let device = match params.get("device_id") {
        Some(r) => r,
        None => return handle_error("argument device_id is required"),
    };
    let mut house = req.state().write().unwrap();

    let mut visitor = SwitchStatusVisitor::new(vec![device]);
    match house.visit_devices_mut(&mut visitor, Some(room)) {
        Ok(_) => {}
        Err(e) => {
            return handle_error(format!("error: {}", e).as_str());
        }
    }
    Ok(Redirect::new("/").into())
}

async fn handle_root(req: Request<Arc<RwLock<HouseImpl>>>) -> tide::Result {
    let house = req.state().read().unwrap();

    let mut reporter = ReportVisitor::default();
    _ = house.visit_devices(&mut reporter, None);

    let mut tera_ctx = tera::Context::new();

    tera_ctx.insert("reports", &reporter.reports);

    let device_types = vec!["socket", "thermometer"];
    tera_ctx.insert("device_types", &device_types);

    let tpl_name = "index.html";
    let tpl_data = include_str!("response_tmpl/index.html");
    let mut tera = Tera::default();
    tera.add_raw_template(tpl_name, tpl_data).unwrap();

    let html = match tera.render(tpl_name, &tera_ctx) {
        Ok(t) => t,
        Err(e) => format!("Render error: {:?}", e),
    };
    let response = Response::builder(200)
        .body(html.as_str())
        .content_type(mime::HTML)
        .build();
    Ok(response)
}

#[async_std::main]
async fn main() -> tide::Result<()> {
    let house = Arc::new(RwLock::new(build_house()?));
    let mut server = tide::with_state(house);
    server.at("/").get(handle_root);
    server.at("/switch_status").get(handle_switch_status);

    println!("listening on http://127.0.0.1:8080");
    server.listen("127.0.0.1:8080").await?;
    Ok(())
}
