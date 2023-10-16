use crate::house::room::Room;
use crate::house::traits::{DeviceVisitor, SmartDevice};
use std::collections::HashMap;
use std::io::{Error, ErrorKind};

#[derive(Debug)]
pub struct House<T> {
    pub name: String,
    rooms: HashMap<String, Room<T>>,
}

impl<T> House<T> {
    pub fn new(name: &str) -> Self {
        House {
            name: name.to_string(),
            rooms: HashMap::new(),
        }
    }

    pub fn add_room(&mut self, room_id: &str) -> Result<(), Error> {
        if self.rooms.contains_key(room_id) {
            return Err(Error::new(
                ErrorKind::AlreadyExists,
                "Room with such name already exists",
            ));
        }
        self.rooms.insert(room_id.to_string(), Room::new(room_id));
        Ok(())
    }

    pub fn del_room(&mut self, room_id: &str) -> Result<(), Error> {
        match self.rooms.get(room_id) {
            Some(room) => match room.devices.len() {
                0 => Err(Error::new(
                    ErrorKind::NotFound,
                    "Room is not empty, extract all devices or use 'del_room_force'",
                )),
                _ => {
                    self.rooms.remove(room_id);
                    Ok(())
                }
            },
            None => Err(Error::new(ErrorKind::NotFound, "Room not found")),
        }
    }

    pub fn get_room_ids(&self) -> Vec<&String> {
        Vec::from_iter(self.rooms.keys())
    }

    pub fn add_device<D: SmartDevice + 'static>(
        &mut self,
        room_id: &str,
        device: D,
    ) -> Result<(), Error> where T: From<D> {
        match self.rooms.get_mut(room_id) {
            Some(room) => room.add_device(device),
            None => Err(Error::new(
                ErrorKind::AlreadyExists,
                "Room with such name already exists",
            )),
        }
    }

    pub fn visit_devices_mut<V: DeviceVisitor<T>>(
        &mut self,
        visitor: &mut V,
        room_id: Option<&str>,
    ) -> Result<(), Error> {
        match room_id {
            Some(name) => match self.rooms.get_mut(name) {
                Some(room) => room.visit_devices_mut(visitor),
                None => {
                    return Err(Error::new(
                        ErrorKind::NotFound,
                        "Room with such id doesn't exist",
                    ))
                }
            },
            None => {
                for r in self.rooms.values_mut() {
                    r.visit_devices(visitor);
                }
                return Ok(());
            }
        }
        Ok(())
    }

    pub fn visit_devices<V: DeviceVisitor<T>>(
        &self,
        visitor: &mut V,
        room_id: Option<&str>,
    ) -> Result<(), Error> {
        match room_id {
            Some(name) => match self.rooms.get(name) {
                Some(room) => room.visit_devices(visitor),
                None => {
                    return Err(Error::new(
                        ErrorKind::NotFound,
                        "Room with such id doesn't exist",
                    ))
                }
            },
            None => {
                for r in self.rooms.values() {
                    r.visit_devices(visitor);
                }
                return Ok(());
            }
        }
        Ok(())
    }

    pub fn extract_device<D: 'static>(
        &mut self,
        room_id: &str,
        device_id: &str,
    ) -> Result<D, Error> {
        match self.rooms.get_mut(room_id) {
            Some(room) => room.extract_device(device_id),
            None => Err(Error::new(ErrorKind::NotFound, "Room not found")),
        }
    }
}
