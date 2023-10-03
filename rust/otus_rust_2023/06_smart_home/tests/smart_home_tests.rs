#[cfg(test)]
use smart_home;

use smart_home::smart_devices::Socket;
use smart_home::smart_home::{House, Room};

#[test]
fn test_rooms_count() {
    let mut house = House::default();
    assert_eq!(house.iter_rooms().len(), 0);
    _ = house.add_room(Room::new("t1"));
    assert_eq!(house.iter_rooms().len(), 1);
    _ = house.add_room(Room::new("t2"));
    assert_eq!(house.iter_rooms().len(), 2);
}

#[test]
fn test_same_rooms_error() {
    let mut house = House::default();
    _ = house.add_room(Room::new("t1"));
    assert!(house.add_room(Room::new("t1")).is_err());
}

#[test]
fn test_devices_count() {
    let mut room = Room::new("r1");
    assert_eq!(room.iter_devices().len(), 0);
    _ = room.add_device(Socket::new("d1".to_string()));
    assert_eq!(room.iter_devices().len(), 1);
    _ = room.add_device(Socket::new("d2".to_string()));
    assert_eq!(room.iter_devices().len(), 2);
}
#[test]
fn test_same_devices_error() {
    let mut room = Room::new("r1");
    _ = room.add_device(Socket::new("d1".to_string()));
    assert!(room.add_device(Socket::new("d1".to_string())).is_err());
}
