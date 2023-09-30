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
