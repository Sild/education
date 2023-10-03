use crate::smart_home::DeviceWithID;
use std::any::Any;

pub struct Socket {
    id: String,
    pub is_on: bool,
}

impl DeviceWithID for Socket {
    fn get_id(&self) -> &str {
        &self.id
    }
    fn as_any_mut(&mut self) -> &mut dyn Any
    where
        Self: Sized,
    {
        self
    }
}

impl Socket {
    pub fn new(id: String) -> Self {
        Self { id, is_on: false }
    }
}

pub struct Thermometer {
    id: String,
    pub is_on: bool,
    #[allow(unused)]
    cur_temp: f32,
}

impl DeviceWithID for Thermometer {
    fn get_id(&self) -> &str {
        &self.id
    }
    fn as_any_mut(&mut self) -> &mut dyn Any
    where
        Self: Sized,
    {
        self
    }
}

impl Thermometer {
    pub fn new(id: String) -> Self {
        Self {
            id,
            is_on: false,
            cur_temp: 0.0,
        }
    }
}

impl Socket {
    #[allow(unused)]
    pub fn get_report(&self) -> String {
        format!("type: socket, id: {}, is_on: {}", self.id, self.is_on)
    }
}

impl Thermometer {
    #[allow(unused)]
    pub fn get_report(&self) -> String {
        format!(
            "type: thermometer, id: {}, is_on: {}, cur_temp: {}",
            self.id, self.is_on, self.cur_temp
        )
    }
}
