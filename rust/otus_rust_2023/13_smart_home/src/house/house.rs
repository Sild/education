use crate::house::room::Room;
use crate::house::traits::{DeviceVisitor, SmartDevice};
use std::collections::HashMap;
use std::io::{Error, ErrorKind};

use super::errors::HouseError;

#[derive(Default, Debug)]
pub struct House<T: SmartDevice> {
    rooms: HashMap<String, Room<T>>,
}

impl<T: SmartDevice> House<T> {
    pub fn new() -> Self {
        Self {
            rooms: HashMap::new(),
        }
    }

    pub fn add_room(&mut self, room_name: &str) -> Result<(), HouseError> {
        if room_name.is_empty() {
            return Err(HouseError::EmptyRoomName());
        }
        if self.rooms.contains_key(room_name) {
            return Err(HouseError::RoomAlreadyExists(room_name.to_string()));
        }
        self.rooms.insert(room_name.to_string(), Room::new(room_name));
        Ok(())
    }

    pub fn del_room(&mut self, room_name: &str) -> Result<(), HouseError> {
        match self.rooms.get(room_name) {
            Some(room) => match room.devices.len() {
                0 => Err(HouseError::NonEmptyRoomRemoving(room_name.to_string())),
                _ => {
                    self.rooms.remove(room_name);
                    Ok(())
                }
            },
            None => Err(HouseError::RoomNotFound(room_name.to_string())),
        }
    }

    pub fn get_room_names(&self) -> Vec<&String> {
        Vec::from_iter(self.rooms.keys())
    }

    pub fn add_device(&mut self, room_name: &str, device: T) -> Result<(), HouseError> {
        match self.rooms.get_mut(room_name) {
            Some(room) => room.add_device(device),
            None => Err(HouseError::RoomAlreadyExists(room_name.to_string())),
        }
    }

    pub fn visit_devices_mut(
        &mut self,
        visitor: &mut dyn DeviceVisitor<T>,
        room_name: Option<&str>,
    ) -> Result<(), Error> {
        match room_name {
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
                    r.visit_devices_mut(visitor);
                }
                return Ok(());
            }
        }
        Ok(())
    }

    pub fn visit_devices(
        &self,
        visitor: &mut dyn DeviceVisitor<T>,
        room_name: Option<&str>,
    ) -> Result<(), HouseError> {
        match room_name {
            Some(name) => match self.rooms.get(name) {
                Some(room) => room.visit_devices(visitor),
                None => return Err(HouseError::RoomNotFound(name.to_string())),
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

    pub fn extract_device(&mut self, room_name: &str, device_id: &str) -> Result<T, HouseError> {
        match self.rooms.get_mut(room_name) {
            Some(room) => room.extract_device(device_id),
            None => Err(HouseError::RoomNotFound(room_name.to_string())),
        }
    }
}
