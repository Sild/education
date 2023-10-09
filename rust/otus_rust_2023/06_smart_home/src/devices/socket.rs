use crate::house::traits::SmartDevice;

pub struct Socket {
    id: String,
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
        format!("type: socket, id: {}, is_on: {}", self.id, self.is_on)
    }
}