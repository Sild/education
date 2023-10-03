mod smart_devices;
mod smart_home;
use std::io::Error;

fn main() -> Result<(), Error> {
    let mut house = smart_home::House::default();

    let mut bathroom = smart_home::Room::new("bathroom");
    bathroom.add_device(smart_devices::Thermometer::new("bath_termo1".to_string()))?;
    bathroom.add_device(smart_devices::Socket::new("bath_socket1".to_string()))?;
    house.add_room(bathroom)?;

    let mut living_room = smart_home::Room::new("living_room");
    living_room.add_device(smart_devices::Thermometer::new(
        "living_termo_window".to_string(),
    ))?;
    living_room.add_device(smart_devices::Thermometer::new(
        "living_termo_door".to_string(),
    ))?;
    living_room.add_device(smart_devices::Socket::new("living_socket1".to_string()))?;
    house.add_room(living_room)?;

    for room in house.iter_rooms() {
        let mut room_info = "room: ".to_string() + room.name.as_str() + "\ndevices:\n";
        for dev in room.iter_devices() {
            room_info += &format!(" - {}\n", dev.get_id());
        }
        println!("{}", room_info)
    }

    Ok(())
}
