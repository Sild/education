use std::any::Any;
use std::collections::hash_map::Values;
use std::collections::HashMap;
use std::fmt::Debug;
use std::io::{Error, ErrorKind};

pub trait DeviceWithID {
    fn get_id(&self) -> &str;
    fn as_any_mut(&mut self) -> &mut dyn Any;
}

impl Debug for dyn DeviceWithID {
    fn fmt(&self, f: &mut core::fmt::Formatter<'_>) -> core::fmt::Result {
        write!(f, "DeviceWithId{{{}}}", self.get_id())
    }
}

#[derive(Debug)]
pub struct Room {
    pub name: String,
    devices: HashMap<String, Box<dyn DeviceWithID>>, // device_id->device
}

impl Room {
    pub fn new(name: &str) -> Self {
        Room {
            name: name.to_string(),
            devices: HashMap::new(),
        }
    }
    pub fn add_device<T: DeviceWithID + 'static>(&mut self, device: T) -> Result<(), Error> {
        if self.devices.contains_key(device.get_id()) {
            return Err(Error::new(
                ErrorKind::Other,
                "Device with such name already exists",
            ));
        }
        self.devices
            .insert(device.get_id().to_string(), Box::new(device));
        Ok(())
    }

    pub fn iter_devices(&self) -> Values<'_, String, Box<dyn DeviceWithID>> {
        self.devices.values()
    }

    #[allow(unused)]
    pub fn get_device<T: 'static>(&mut self, device_id: &str) -> Result<&T, Error> {
        match self.devices.get_mut(device_id) {
            Some(trait_device) => match trait_device.as_any_mut().downcast_ref::<T>() {
                Some(device) => Ok(device),
                None => Err(Error::new(ErrorKind::Other, "Wrong device type")),
            },
            None => Err(Error::new(ErrorKind::NotFound, "Device not found")),
        }
    }

    #[allow(unused)]
    pub fn get_device_mut<T: 'static>(&mut self, device_id: &str) -> Result<&mut T, Error> {
        match self.devices.get_mut(device_id) {
            Some(any_device) => match any_device.as_any_mut().downcast_mut::<T>() {
                Some(device) => Ok(device),
                None => Err(Error::new(ErrorKind::Other, "Wrong device type")),
            },
            None => Err(Error::new(ErrorKind::NotFound, "Device not found")),
        }
    }
}

#[derive(Default, Debug)]
pub struct House {
    pub name: String,
    rooms: HashMap<String, Room>,
}

impl House {
    pub fn add_room(&mut self, room: Room) -> Result<(), Error> {
        if self.rooms.contains_key(&room.name) {
            return Err(Error::new(
                ErrorKind::AlreadyExists,
                "Room with such name already exists",
            ));
        }
        self.rooms.insert(room.name.clone(), room);
        Ok(())
    }

    pub fn iter_rooms(&self) -> Values<'_, String, Room> {
        self.rooms.values()
    }

    #[allow(unused)]
    pub fn get_room(&self, room_id: &str) -> Option<&Room> {
        self.rooms.get(room_id)
    }

    pub fn get_room_mut(&mut self, room_id: &str) -> Option<&mut Room> {
        self.rooms.get_mut(room_id)
    }
}
