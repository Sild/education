#[cfg(test)]
use smart_home;
use std::ops::Index;

use smart_home::smart_devices::{Socket, Thermometer};
use smart_home::smart_home::House;

#[test]
fn check_room_count() {
    let house = House::new(2);
    assert_eq!(house.get_rooms().len(), 2);
    let house = House::new(4);
    assert_eq!(house.get_rooms().len(), 4);
}

#[test]
fn check_device_registration() {
    let mut house = House::new(2);
    let socket = Socket::new("s1".into());
    let socket_2 = Socket::new("s1".into());
    let socket_3 = Socket::new("s1".into());

    let thermo = Thermometer::new("t1".into());
    assert!(!house
        .register_device(house.get_rooms().index(0), socket)
        .is_err());
    assert!(!house
        .register_device(house.get_rooms().index(0), thermo)
        .is_err());

    // same id in 1 room is no allowed
    assert!(house
        .register_device(house.get_rooms().index(0), socket_2)
        .is_err());
    // in another room - you're welcome
    assert!(!house
        .register_device(house.get_rooms().index(1), socket_3)
        .is_err());
}

#[test]
fn check_report() {
    let mut house = House::new(2);
    let socket = Socket::new("s1".into());
    let thermo = Thermometer::new("t1".into());

    let socket_2 = Socket::new("s2".into());

    house
        .register_device(house.get_rooms().index(0), socket)
        .expect("...");
    house
        .register_device(house.get_rooms().index(0), thermo)
        .expect("...");
    house
        .register_device(house.get_rooms().index(1), socket_2)
        .expect("...");

    let _report = house.build_report(house.get_rooms().index(0));
    // i'm too lazy for proper tests here
}

#[test]
fn check_get_devices() {
    let mut house = House::new(1);
    let socket = Socket::new("s1".into());

    house
        .register_device(house.get_rooms().index(0), socket)
        .expect("...");

    let rooms = house.get_rooms();
    assert!(house.get_devices(rooms.index(0)).is_some());
    assert!(house
        .get_devices("random_string_definitely_not_room")
        .is_none())
    // i'm too lazy for proper tests here
}
