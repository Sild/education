use crate::devices::IdType;
use crate::house::traits::SmartDevice;

pub struct Thermo {
    id: IdType,
    pub is_on: bool,
    #[allow(unused)]
    cur_temp: f32,
}

impl SmartDevice for Thermo {
    fn get_id(&self) -> &str {
        &self.id
    }
}

impl Thermo {
    pub fn new(id: String) -> Self {
        Self {
            id,
            is_on: false,
            cur_temp: 0.0,
        }
    }
}

impl Thermo {
    #[allow(unused)]
    pub fn get_report(&self) -> String {
        format!(
            "type: thermometer, id: {}, is_on: {}, cur_temp: {}",
            self.id, self.is_on, self.cur_temp
        )
    }
}
