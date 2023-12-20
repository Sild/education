use crate::house::traits::SmartDevice;

pub mod socket;
pub mod thermo;
pub mod visitors;
pub type IdType = String;

pub enum Device {
    Socket(socket::Socket),
    Thermo(thermo::Thermo),
}

impl SmartDevice for Device {
    fn get_id(&self) -> &str {
        match self {
            Device::Socket(device) => device.get_id(),
            Device::Thermo(device) => device.get_id(),
        }
    }
}

impl From<socket::Socket> for Device {
    fn from(device: socket::Socket) -> Self {
        Device::Socket(device)
    }
}

impl From<thermo::Thermo> for Device {
    fn from(device: thermo::Thermo) -> Self {
        Device::Thermo(device)
    }
}
