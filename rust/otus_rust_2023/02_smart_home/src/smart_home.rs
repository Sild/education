use std::io::Error;

#[derive(Debug)]
pub struct Socket {}

impl Socket {
    pub fn _new() -> Self {
        Socket {}
    }
    pub fn _description() -> &'static str {
        "this is smart socket"
    }

    pub fn _turn_on() -> Result<(), Error> {
        todo!();
    }

    pub fn _turn_off() -> Result<(), Error> {
        todo!();
    }

    pub fn _get_watts() -> Result<f32, Error> {
        todo!()
    }
}

#[derive(Debug)]
pub struct _Thermometer {}

impl _Thermometer {
    pub fn _get_cur_degree() -> Result<f32, Error> {
        todo!()
    }
}
