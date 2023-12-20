use std::any::Any;
use std::fmt::Debug;

pub trait SmartDevice: Sync + Send {
    fn get_id(&self) -> &str;
}

pub trait DeviceVisitor {
    fn visit(&mut self, _room_id: &str, _any_device: &dyn Any) {}
    fn visit_mut(&mut self, _room_id: &str, _any_device: &mut dyn Any) {}
}

impl Debug for dyn SmartDevice {
    fn fmt(&self, f: &mut core::fmt::Formatter<'_>) -> core::fmt::Result {
        write!(f, "DeviceWithId{{{}}}", self.get_id())
    }
}
