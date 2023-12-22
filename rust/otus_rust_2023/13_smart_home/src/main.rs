#![feature(let_chains)]

use smart_home::devices::visitors::{ReportVisitor, SwitchStatusVisitor};
use smart_home::devices::Device;
use smart_home::house::house::House;
use std::collections::HashMap;
use std::fs;
use std::sync::{Arc, RwLock};
use tera::Tera;
use tide::http::mime;

use smart_home::devices::socket::Socket;
use smart_home::devices::thermo::Thermo;
use tide::{Redirect, Request, Response, Result, StatusCode};
type HouseImpl = House<Device>;

#[allow(clippy::upper_case_acronyms)]
enum ResponseFormat {
    HTML,
    TXT,
}

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

fn render(
    req: &Request<Arc<RwLock<HouseImpl>>>,
    tpl_name: &str,
    status_code: tide::StatusCode,
    tera_ctx: &tera::Context,
) -> tide::Result {
    let params: HashMap<String, String> = req.query()?;
    let mut format = ResponseFormat::HTML;
    if let Some(fmt) = params.get("fmt")
        && fmt == "txt"
    {
        format = ResponseFormat::TXT;
    }

    let tpl_suffix = match format {
        ResponseFormat::HTML => "html",
        ResponseFormat::TXT => "txt",
    };
    let tpl_data = match fs::read_to_string(
        format!(
            "{}/src/response_tmpl/{tpl_name}.{tpl_suffix}",
            project_root::get_project_root()
                .unwrap()
                .as_path()
                .display()
        )
        .as_str(),
    ) {
        Ok(t) => t,
        Err(e) => {
            println!("Error reading template: {}", e);
            format!("Fail to read template: {}", e).to_string()
        }
    };
    let mut tera = Tera::default();
    tera.add_raw_template(tpl_name, tpl_data.as_str()).unwrap();

    let html = match tera.render(tpl_name, tera_ctx) {
        Ok(t) => t,
        Err(e) => format!("Render error: {:?}", e),
    };
    let response = Response::builder(status_code)
        .body(html.as_str())
        .content_type(match format {
            ResponseFormat::HTML => mime::HTML,
            ResponseFormat::TXT => mime::PLAIN,
        })
        .build();
    Ok(response)
}

fn handle_error(req: &Request<Arc<RwLock<HouseImpl>>>, err: &str) -> tide::Result {
    let mut tera_ctx = tera::Context::new();
    tera_ctx.insert("error_text", &err.to_string());
    render(req, "error", StatusCode::BadRequest, &tera_ctx)
}

fn handle_redirect(req: &Request<Arc<RwLock<HouseImpl>>>) -> tide::Result {
    let params: HashMap<String, String> = req.query()?;
    if let Some(fmt) = params.get("fmt")
        && fmt == "txt"
    {
        return Ok(Redirect::new("/?fmt=txt").into());
    }
    Ok(Redirect::new("/").into())
}

async fn handle_switch_status(req: Request<Arc<RwLock<HouseImpl>>>) -> tide::Result {
    let params: HashMap<String, String> = req.query()?;
    let room = match params.get("room_name") {
        Some(r) => r,
        None => return handle_error(&req, "argument room_name is required"),
    };
    let device = match params.get("device_id") {
        Some(r) => r,
        None => return handle_error(&req, "argument device_id is required"),
    };
    let mut house = req.state().write().unwrap();

    let mut visitor = SwitchStatusVisitor::new(vec![device]);
    match house.visit_devices_mut(&mut visitor, Some(room)) {
        Ok(_) => {}
        Err(e) => {
            return handle_error(&req, format!("error: {}", e).as_str());
        }
    }
    handle_redirect(&req)
}

async fn handle_root(req: Request<Arc<RwLock<HouseImpl>>>) -> tide::Result {
    // collect data
    let mut reporter = ReportVisitor::default();
    {
        let house = req.state().read().unwrap();
        _ = house.visit_devices(&mut reporter, None);
        for room in house.get_room_names() {
            if !reporter.reports.contains_key(room) {
                _ = reporter.reports.insert(room.clone(), HashMap::default())
            }
        }
    }
    let device_types = vec!["socket", "thermometer"];

    // populate ctx
    let mut tera_ctx = tera::Context::new();
    tera_ctx.insert("reports", &reporter.reports);
    tera_ctx.insert("device_types", &device_types);
    render(&req, "index", tide::StatusCode::Ok, &tera_ctx)
}

async fn handle_add_room(mut req: Request<Arc<RwLock<HouseImpl>>>) -> tide::Result {
    let params: HashMap<String, String> = req.body_form().await?;
    let room = match params.get("room_name") {
        Some(r) => r,
        None => return handle_error(&req, "argument room_name is required"),
    };
    {
        let mut house = req.state().write().unwrap();
        match house.add_room(room) {
            Ok(_) => {}
            Err(e) => {
                return handle_error(&req, format!("error: {}", e).as_str());
            }
        }
    }
    handle_redirect(&req)
}

async fn handle_add_device(mut req: Request<Arc<RwLock<HouseImpl>>>) -> tide::Result {
    let params: HashMap<String, String> = req.body_form().await?;
    let room = match params.get("room_name") {
        Some(r) => r,
        None => return handle_error(&req, "argument room_name is required"),
    };
    let device_id = match params.get("device_id") {
        Some(r) => r,
        None => return handle_error(&req, "argument device_id is required"),
    };
    let device = match params.get("device_type") {
        Some(r) => match r.as_str() {
            "socket" => Device::Socket(Socket::new(device_id.to_string())),
            "thermometer" => Device::Thermo(Thermo::new(device_id.to_string())),
            _ => return handle_error(&req, "argument device_type is invalid"),
        },
        None => return handle_error(&req, "argument device_type is required"),
    };
    let mut house: std::sync::RwLockWriteGuard<'_, House<Device>> = req.state().write().unwrap();
    match house.add_device(room, device) {
        Ok(_) => {}
        Err(e) => {
            return handle_error(&req, format!("error: {}", e).as_str());
        }
    }
    handle_redirect(&req)
}

async fn handle_remove_room(req: Request<Arc<RwLock<HouseImpl>>>) -> tide::Result {
    let params: HashMap<String, String> = req.query()?;
    let room = match params.get("room_name") {
        Some(r) => r,
        None => return handle_error(&req, "argument room_name is required"),
    };
    let mut house: std::sync::RwLockWriteGuard<'_, House<Device>> = req.state().write().unwrap();
    match house.del_room(room) {
        Ok(_) => {}
        Err(e) => {
            return handle_error(&req, format!("error: {}", e).as_str());
        }
    }
    handle_redirect(&req)
}

async fn handle_remove_device(req: Request<Arc<RwLock<HouseImpl>>>) -> tide::Result {
    let params: HashMap<String, String> = req.query()?;
    let room = match params.get("room_name") {
        Some(r) => r,
        None => return handle_error(&req, "argument room_name is required"),
    };
    let device_id = match params.get("device_id") {
        Some(r) => r,
        None => return handle_error(&req, "argument device_id is required"),
    };
    let mut house: std::sync::RwLockWriteGuard<'_, House<Device>> = req.state().write().unwrap();
    match house.extract_device(room, device_id) {
        Ok(_) => {}
        Err(e) => {
            return handle_error(&req, format!("error: {}", e).as_str());
        }
    }
    handle_redirect(&req)
}

#[async_std::main]
async fn main() -> tide::Result<()> {
    let house = Arc::new(RwLock::new(build_house()?));
    let mut server = tide::with_state(house);
    server.at("/").get(handle_root);
    server.at("/switch_status").get(handle_switch_status);
    server.at("/add_room").post(handle_add_room);
    server.at("/add_device").post(handle_add_device);
    server.at("/remove_room").get(handle_remove_room);
    server.at("/remove_device").get(handle_remove_device);
    server.at("*").all(|_| async { Ok("Unknown path") });

    println!("listening on http://127.0.0.1:8080");
    server.listen("127.0.0.1:8080").await?;
    Ok(())
}
