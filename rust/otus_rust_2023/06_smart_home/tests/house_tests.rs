#[cfg(test)]
use std::any::Any;
use smart_home;
use smart_home::devices::socket::Socket;
use smart_home::house::house::House;
use smart_home::house::traits::{DeviceVisitor, SmartDevice};

#[test]
fn test_rooms_count() {
    let mut house = House::default();
    assert_eq!(house.get_room_ids().len(), 0);
    _ = house.add_room("t1");
    assert_eq!(house.get_room_ids().len(), 1);
    _ = house.add_room("t2");
    assert_eq!(house.get_room_ids().len(), 2);
}

#[test]
fn test_same_rooms_error() {
    let mut house = House::default();
    _ = house.add_room("t1");
    assert!(house.add_room("t1").is_err());
}

#[test]
fn test_same_devices_error() {
    let mut house = House::default();
    _ = house.add_room("t1");
    _ = house.add_room("t2");
    _ = house.add_device("t1", Socket::new("d1".to_string()));
    assert!(house.add_device("t1", Socket::new("d1".to_string())).is_err());
    // fine for another room
    assert!(house.add_device("t2", Socket::new("d1".to_string())).is_ok());
}

#[test]
fn test_wrong_room_add_device() {
    let mut house = House::default();
    _ = house.add_room("t1");
    assert!(house.add_device("t2", Socket::new("d1".to_string())).is_err());
}

#[test]
fn test_visitor() {
    struct TestDevice {
        id: String
    }
    impl SmartDevice for TestDevice {
        fn get_id(&self) -> &str {
            self.id.as_str()
        }
    }
    #[derive(Default)]
    struct TestVisitor {
        data: Vec<String>
    }
    impl DeviceVisitor for TestVisitor {
        fn visit(&mut self, room_id: &str, any_device: &dyn Any) {
            self.data.push(format!("{}_{}", room_id, any_device.downcast_ref::<TestDevice>().expect("fail to downcast stored device").get_id()))
        }
        fn visit_mut(&mut self, room_id: &str, any_device: &mut dyn Any) {
            any_device.downcast_mut::<TestDevice>().expect("fail to downcast stored device").id = format!("{}_{}", room_id, "new_id")
        }
    }

    let mut house = House::default();
    _ = house.add_room("r1");
    _ = house.add_device("r1", TestDevice{id: "d1".to_string()});

    _ = house.add_room("r2");
    _ = house.add_device("r2", TestDevice{id: "d1".to_string()});
    _ = house.add_device("r2", TestDevice{id: "d2".to_string()});

    // error
    let mut visitor = TestVisitor::default();
    assert!(house.visit_devices(&mut visitor, Some("r0")).is_err());

    // visit 3 devices
    let mut visitor = TestVisitor::default();
    assert!(house.visit_devices(&mut visitor, None).is_ok());
    assert_eq!(visitor.data.len(), 3);

    // visit 1 device
    let mut visitor = TestVisitor::default();
    assert!(house.visit_devices(&mut visitor, Some("r1")).is_ok());
    assert_eq!(visitor.data.len(), 1);

    // visit 2 devices
    let mut visitor = TestVisitor::default();
    assert!(house.visit_devices(&mut visitor, Some("r2")).is_ok());
    assert_eq!(visitor.data.len(), 2);

    // visit 2 devices
    let mut visitor = TestVisitor::default();
    assert!(house.visit_devices_mut(&mut visitor, Some("r2")).is_ok());
    assert_eq!(visitor.data.len(), 0); // TestVisitor::visit_mut doesn't update data

    // check ids are updated
    // really stupid feature - I changed device ID, but house doesn't know about it ¯\_(ツ)_/¯
    assert!(house.visit_devices(&mut visitor, Some("r2")).is_ok());
    assert!(visitor.data[0].contains("r2_new_id"));
    assert!(visitor.data[1].contains("r2_new_id"));
}
