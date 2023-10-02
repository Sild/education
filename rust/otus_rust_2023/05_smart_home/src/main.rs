mod smart_devices;
mod smart_home;
use rand::Rng;
use std::io::Error;

const MAX_ROOMS: i32 = 4;

fn main() -> Result<(), Error> {
    let mut rng = rand::thread_rng();
    let rooms_cnt = rng.gen_range(1..=MAX_ROOMS);
    let mut house = smart_home::House::new(rooms_cnt as usize);

    let rooms = house.get_rooms();

    for room in rooms.iter() {
        let socket1 = smart_devices::Socket::new(format!("{}_socket_{}", room, 1));
        house.register_device(room, socket1)?;
        let mut thermo1 = smart_devices::Thermometer::new(format!("{}_thermo_{}", room, 1));
        thermo1.is_on = true;
        house.register_device(room, thermo1)?;
    }

    for room in rooms.iter() {
        let room_report = house.build_report(room)?;
        println!("Room '{}' report:\n{}\n", room, room_report.join("\n"));
    }

    Ok(())
}
