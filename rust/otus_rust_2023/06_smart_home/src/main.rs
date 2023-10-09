use smart_home::devices::socket::Socket;
use smart_home::devices::thermo::Thermometer;
use smart_home::devices::visitors::{ReportVisitor, TurnOnVisitor};
use smart_home::house::house::House;
use std::io::Error;

fn print_device_report(house: &House, report_tag: &str) {
    println!("\n{}", report_tag);
    let mut reporter = ReportVisitor::default();
    _ = house.visit_devices(&mut reporter, None);
    reporter.print_report();
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

    print_device_report(&house, "===default report===");

    let mut turn_on = TurnOnVisitor::default();
    _ = house.visit_devices_mut(&mut turn_on, Some(bathroom));

    print_device_report(&house, "===turn_on report===");

    _ = house.extract_device::<Socket>(bathroom, "bath_socket1");

    print_device_report(&house, "===extract report===");

    Ok(())
}
