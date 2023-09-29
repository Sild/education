use crate::smart_home::SmartDevice;

pub struct Socket {
    id: String,
    pub is_on: bool,
}
impl Socket {
    pub(crate) fn new(id: String) -> Self {
        Self { id, is_on: false }
    }
}

pub struct Thermometer {
    id: String,
    pub is_on: bool,
    cur_temp: f32,
}

impl Thermometer {
    pub(crate) fn new(id: String) -> Self {
        Self {
            id,
            is_on: false,
            cur_temp: 0.0,
        }
    }
}

impl SmartDevice for Socket {
    fn get_report(&self) -> String {
        format!("type: socket, id: {}, is_on: {}", self.id, self.is_on)
    }

    fn get_id(&self) -> &str {
        self.id.as_str()
    }
}

impl SmartDevice for Thermometer {
    fn get_report(&self) -> String {
        format!(
            "type: thermometer, id: {}, is_on: {}, cur_temp: {}",
            self.id, self.is_on, self.cur_temp
        )
    }

    fn get_id(&self) -> &str {
        self.id.as_str()
    }
}

mod tests {
    use crate::smart_devices::{Socket, Thermometer};
    use crate::smart_home::SmartDevice;

    #[test]
    fn check_trait_data() {
        let id = "test";
        let sock = Socket::new(id.to_string());
        assert_eq!(sock.get_id(), id);
        assert!(sock.get_report().contains("socket"));
        let mut therm = Thermometer::new(id.to_string());
        assert_eq!(therm.get_id(), id);
        assert!(therm.get_report().contains("thermometer"));
        therm.is_on = true;
        assert!(therm.get_report().contains("is_on: true"));
        assert!(therm.get_report().contains("cur_temp: "));
    }
}
