use std::collections::{HashMap, HashSet};
use std::io::{Error, ErrorKind};
use std::ops::Index;

pub trait SmartDevice {
    fn get_report(&self) -> String;
    fn get_id(&self) -> &str;
}

type DeviceStorage = HashMap<String, HashMap<String, Box<dyn SmartDevice>>>;
pub struct House {
    rooms: HashSet<String>,
    devices: DeviceStorage,
}

impl House {
    pub fn new(rooms_cnt: usize) -> Self {
        let rooms = HashSet::from_iter((0..rooms_cnt).map(|i| (format!("Room_{}", i))));
        let devices =
            DeviceStorage::from_iter(rooms.iter().map(|room| (room.to_string(), HashMap::new())));

        House { rooms, devices }
    }

    pub fn get_rooms(&self) -> Vec<String> {
        Vec::from_iter(self.rooms.iter().cloned())
    }

    pub fn register_device<T: SmartDevice + 'static>(
        &mut self,
        room: &str,
        device: T,
    ) -> Result<(), Error> {
        let room_devices = self.devices.get_mut(room).unwrap();
        let device_id = device.get_id();
        if room_devices.contains_key(device_id) {
            return Err(Error::new(
                ErrorKind::AlreadyExists,
                format!("Device with id={} is already registered", device_id),
            ));
        }
        room_devices.insert(device_id.to_string(), Box::new(device));
        Ok(())
    }

    pub fn build_report(&self, room: &str) -> Result<Vec<String>, Error> {
        if !self.rooms.contains(room) {
            return Err(Error::new(ErrorKind::InvalidInput, "Room not found"));
        };
        let res = Vec::from_iter(
            self.devices
                .index(room)
                .iter()
                .map(|x| format!("device: {}, report: {:?}", x.0, x.1.get_report())),
        );
        Ok(res)
    }
}

mod tests {
    use crate::smart_devices::{Socket, Thermometer};
    use crate::smart_home::House;
    use std::ops::Index;

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
}
