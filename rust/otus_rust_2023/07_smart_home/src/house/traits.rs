use std::any::Any;
use std::fmt::Debug;

pub trait SmartDevice {
    fn get_id(&self) -> &str;
}

pub trait DeviceVisitor<T> {
    fn visit(&mut self, _room_id: &str, _any_device: &T) {}
    fn visit_mut(&mut self, _room_id: &str, _any_device: &mut T) {}
}

impl Debug for dyn SmartDevice {
    fn fmt(&self, f: &mut core::fmt::Formatter<'_>) -> core::fmt::Result {
        write!(f, "DeviceWithId{{{}}}", self.get_id())
    }
}
