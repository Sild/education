use smart_home;
use smart_home::devices::socket::Socket;
use smart_home::devices::Device;
use smart_home::house::house::House;



#[test]
fn test_rooms_count() {
    let mut house = House::<Device>::new();
    assert_eq!(house.get_room_ids().len(), 0);
    _ = house.add_room("t1");
    assert_eq!(house.get_room_ids().len(), 1);
    _ = house.add_room("t2");
    assert_eq!(house.get_room_ids().len(), 2);
}

#[test]
fn test_same_rooms_error() {
    let mut house = House::<Device>::new();
    _ = house.add_room("t1");
    assert!(house.add_room("t1").is_err());
}

#[test]
fn test_same_devices_error() {
    let mut house = House::<Device>::new();
    _ = house.add_room("t1");
    _ = house.add_room("t2");
    _ = house.add_device("t1", Socket::new("d1".to_string()).into());
    assert!(house
        .add_device("t1", Socket::new("d1".to_string()).into())
        .is_err());
    // fine for another room
    assert!(house
        .add_device("t2", Socket::new("d1".to_string()).into())
        .is_ok());
}

#[test]
fn test_wrong_room_add_device() {
    let mut house = House::<Device>::new();
    _ = house.add_room("t1");
    assert!(house
        .add_device("t2", Socket::new("d1".to_string()).into())
        .is_err());
}
