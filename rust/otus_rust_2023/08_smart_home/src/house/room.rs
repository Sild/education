use crate::house::traits::{DeviceVisitor, SmartDevice};
use std::any::Any;
use std::collections::HashMap;
use std::io::{Error, ErrorKind};

#[derive(Debug)]
pub struct Room {
    name: String,
    pub(crate) devices: HashMap<String, Box<dyn Any>>, // device_id->device
}

impl Room {
    pub fn new(name: &str) -> Self {
        Room {
            name: name.to_string(),
            devices: HashMap::new(),
        }
    }
    pub(crate) fn add_device<T: SmartDevice + 'static>(&mut self, device: T) -> Result<(), Error> {
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

    pub(crate) fn extract_device<T: 'static>(&mut self, device_id: &str) -> Result<T, Error> {
        match self.devices.remove(device_id) {
            Some(any_device) => {
                match any_device.downcast::<T>() {
                    Ok(device) => Ok(*device),
                    Err(any_device_back) => {
                        // not grate - push device back to storage ¯\_(ツ)_/¯
                        self.devices.insert(device_id.to_string(), any_device_back);
                        Err(Error::new(ErrorKind::InvalidInput, "wrong device type"))
                    }
                }
            }
            None => Err(Error::new(ErrorKind::NotFound, "Device not found")),
        }
    }

    pub(crate) fn visit_devices_mut<T: DeviceVisitor>(&mut self, visitor: &mut T) {
        for d in self.devices.values_mut() {
            // it's tricky, visitor.visit() can accept anything. We must unwrap the Box before call.
            visitor.visit_mut(self.name.as_str(), &mut (**d))
        }
    }

    pub fn visit_devices<T: DeviceVisitor>(&self, visitor: &mut T) {
        for d in self.devices.values() {
            visitor.visit(self.name.as_str(), &(**d))
        }
    }
}
