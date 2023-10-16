use crate::house::traits::{DeviceVisitor, SmartDevice};
use std::collections::HashMap;
use std::io::{Error, ErrorKind};
use crate::devices::socket::Socket;
use crate::devices::Type;

#[derive(Debug)]
pub struct Room<T> {
    name: String,
    pub(crate) devices: HashMap<String, T>, // device_id->device
}

impl<T> Room<T> {
    pub fn new(name: &str) -> Self {
        Room {
            name: name.to_string(),
            devices: HashMap::new(),
        }
    }
    pub(crate) fn add_device<D: SmartDevice + 'static>(&mut self, device: D) -> Result<(), Error>
    where T: From<D> {
        if self.devices.contains_key(device.get_id()) {
            return Err(Error::new(
                ErrorKind::Other,
                "Device with such name already exists",
            ));
        }
        self.devices
            .insert(device.get_id().to_string(), device.into());
        Ok(())
    }

    pub(crate) fn extract_device<D: 'static>(&mut self, device_id: &str) -> Result<D, Error> {
        match self.devices.remove(device_id) {
            Some(device) => {
                match device.try_into() {
                    Ok(d) => Ok(d),
                    Err(device_back) => {
                        // not grate - push device back to storage ¯\_(ツ)_/¯
                        self.devices.insert(device_id.to_string(), device_back);
                        Err(Error::new(ErrorKind::InvalidInput, "wrong device type"))
                    }
                }
            },
            None => Err(Error::new(ErrorKind::NotFound, "Device not found")),
        }
    }

    pub(crate) fn visit_devices_mut<V: DeviceVisitor<T>>(&mut self, visitor: &mut V) {
        for d in self.devices.values_mut() {
            // it's tricky, visitor.visit() can accept anything. We must unwrap the Box before call.
            visitor.visit_mut(self.name.as_str(), d)
        }
    }

    pub fn visit_devices<V: DeviceVisitor<T>>(&self, visitor: &mut V) {
        for d in self.devices.values() {
            visitor.visit(self.name.as_str(), d)
        }
    }
}
