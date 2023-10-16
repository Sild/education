pub mod socket;
pub mod thermo;
pub mod visitors;

pub enum Type {
    Socket(socket::Socket),
    Thermo(thermo::Thermo)
}