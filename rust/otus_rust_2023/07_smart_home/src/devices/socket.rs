use crate::house::traits::SmartDevice;
use rand::Rng;
use crate::devices::IdType;

pub struct Socket {
    id: IdType,
    pub is_on: bool,
}

impl SmartDevice for Socket {
    fn get_id(&self) -> &str {
        &self.id
    }
}

impl Socket {
    pub fn new(id: String) -> Self {
        Self { id, is_on: false }
    }
}

impl Socket {
    #[allow(unused)]
    pub fn get_report(&self) -> String {
        let mut rng = rand::thread_rng();
        let power = match self.is_on {
            true => rng.gen::<f64>(),
            false => 0.0,
        };
        format!("type: socket, id: {}, is_on: {}, power: {power:.2}", self.id, self.is_on)
    }
}
