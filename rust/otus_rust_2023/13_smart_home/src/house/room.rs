use crate::house::errors::HouseError;
use crate::house::traits::{DeviceVisitor, SmartDevice};
use std::collections::HashMap;

#[derive(Debug)]
pub struct Room<T: SmartDevice> {
    name: String,
    pub(crate) devices: HashMap<String, T>, // device_id->device
}

impl<T: SmartDevice> Room<T> {
    pub fn new(name: &str) -> Self {
        Room {
            name: name.to_string(),
            devices: HashMap::new(),
        }
    }
    pub(crate) fn add_device(&mut self, device: T) -> Result<(), HouseError> {
        if self.devices.contains_key(device.get_id()) {
            return Err(HouseError::DeviceAlreadyExists(device.get_id().to_string()));
        }
        self.devices.insert(device.get_id().to_string(), device);
        Ok(())
    }

    pub(crate) fn extract_device(&mut self, device_id: &str) -> Result<T, HouseError> {
        match self.devices.remove(device_id) {
            Some(device) => Ok(device),
            _ => Err(HouseError::DeviceNotFound(device_id.to_string())),
        }
    }

    pub(crate) fn visit_devices_mut(&mut self, visitor: &mut dyn DeviceVisitor<T>) {
        for d in self.devices.values_mut() {
            visitor.visit_mut(self.name.as_str(), d)
        }
    }

    pub fn visit_devices(&self, visitor: &mut dyn DeviceVisitor<T>) {
        for d in self.devices.values() {
            visitor.visit(self.name.as_str(), d)
        }
    }
}
