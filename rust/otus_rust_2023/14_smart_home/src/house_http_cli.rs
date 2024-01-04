use std::env;
use std::io;
use std::io::Write;
use std::str::FromStr;
use strum::IntoEnumIterator;
use strum_macros::EnumIter;

#[allow(non_camel_case_types)]
#[derive(Debug, PartialEq, EnumIter)]
enum Actions {
    get_state,
    add_room,
    add_device,
    del_device,
    del_room,
    switch_status,
}

impl FromStr for Actions {
    type Err = ();

    fn from_str(input: &str) -> Result<Actions, Self::Err> {
        match input {
            "get_state" => Ok(Actions::get_state),
            "add_room" => Ok(Actions::add_room),
            "add_device" => Ok(Actions::add_device),
            "del_room" => Ok(Actions::del_room),
            "del_device" => Ok(Actions::del_device),
            "switch_status" => Ok(Actions::switch_status),
            _ => Err(()),
        }
    }
}

fn read_stdin(var_name: &str) -> String {
    print!("Enter {var_name}: ");
    io::stdout().flush().unwrap();
    let mut res = String::new();
    std::io::stdin().read_line(&mut res).unwrap();
    res.trim().to_string()
}

fn print_response(rsp: &str) {
    println!("====START_RESPONSE====\n{rsp}\n====END_RESPONSE====");
}

fn get_state(url: &str) {
    let req_url = url.to_string() + "?fmt=txt";
    let rsp: String = ureq::get(req_url.as_str())
        .call()
        .unwrap()
        .into_string()
        .unwrap();

    print_response(rsp.as_str());
}

fn add_room(url: &str) {
    let room_name = read_stdin("room_name");
    let req_url = url.to_string() + "/add_room?fmt=txt";

    let rsp = match ureq::post(req_url.as_str()).send_form(&[("room_name", room_name.as_str())]) {
        Ok(r) => r.into_string().unwrap(),
        Err(e) => match e.into_response() {
            Some(r) => r.into_string().unwrap(),
            None => "error during parsing the response".to_string(),
        },
    };
    print_response(rsp.as_str());
}

fn remove_room(url: &str) {
    let room_name = read_stdin("room_name");
    let req_url = url.to_string() + format!("/remove_room?room_name={room_name}&fmt=txt").as_str();

    let rsp: String = match ureq::get(req_url.as_str()).call() {
        Ok(r) => r.into_string().unwrap(),
        Err(e) => match e.into_response() {
            Some(r) => r.into_string().unwrap(),
            None => "error during parsing the response".to_string(),
        },
    };
    print_response(rsp.as_str());
}

fn remove_device(url: &str) {
    let room_name = read_stdin("room_name");
    let device_id = read_stdin("device_id");
    let req_url = url.to_string()
        + format!("/remove_device?room_name={room_name}&device_id={device_id}&fmt=txt").as_str();

    let rsp: String = match ureq::get(req_url.as_str()).call() {
        Ok(r) => r.into_string().unwrap(),
        Err(e) => match e.into_response() {
            Some(r) => r.into_string().unwrap(),
            None => "error during parsing the response".to_string(),
        },
    };
    print_response(rsp.as_str());
}

fn add_device(url: &str) {
    let room_name = read_stdin("room_name");
    let device_id = read_stdin("device_id");
    let device_type = read_stdin("device_type (socket either thermometer)");

    let req_url = url.to_string() + "/add_device?fmt=txt";

    let rsp = match ureq::post(req_url.as_str()).send_form(&[
        ("room_name", room_name.as_str()),
        ("device_id", device_id.as_str()),
        ("device_type", device_type.as_str()),
    ]) {
        Ok(r) => r.into_string().unwrap(),
        Err(e) => match e.into_response() {
            Some(r) => r.into_string().unwrap(),
            None => "error during parsing the response".to_string(),
        },
    };
    print_response(rsp.as_str());
}

fn switch_status(url: &str) {
    let room_name = read_stdin("room_name");
    let device_id = read_stdin("device_id");
    let req_url = url.to_string()
        + format!("/switch_status?room_name={room_name}&device_id={device_id}&fmt=txt").as_str();

    let rsp: String = match ureq::get(req_url.as_str()).call() {
        Ok(r) => r.into_string().unwrap(),
        Err(e) => match e.into_response() {
            Some(r) => r.into_string().unwrap(),
            None => "error during parsing the response".to_string(),
        },
    };
    print_response(rsp.as_str());
}

fn main() {
    let args: Vec<_> = env::args().collect();
    if args.len() != 2 {
        println!("Usage: ./cli http://localhost:8080");
        return;
    }
    let house_addr = &args[1];
    println!("Cur state:");
    get_state(house_addr);

    loop {
        println!("Available actions: ");
        for act in Actions::iter() {
            println!("{:?}", act);
        }
        print!("\nYour choice: ");
        io::stdout().flush().unwrap();
        let mut action = String::new();
        std::io::stdin().read_line(&mut action).unwrap();
        let action = action.trim();
        println!("Selected action: {}", action);
        match Actions::from_str(action) {
            Ok(Actions::get_state) => get_state(house_addr),
            Ok(Actions::add_room) => add_room(house_addr),
            Ok(Actions::add_device) => add_device(house_addr),
            Ok(Actions::del_room) => remove_room(house_addr),
            Ok(Actions::del_device) => remove_device(house_addr),
            Ok(Actions::switch_status) => switch_status(house_addr),
            _ => println!("Unknown action: {}!", action),
        }
    }
}
