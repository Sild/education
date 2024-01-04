use crate::devices::IdType;
use rand::Rng;

pub struct Socket {
    id: IdType,
    pub is_on: bool,
}

impl Socket {
    pub fn get_id(&self) -> &str {
        &self.id
    }

    pub fn new(id: String) -> Self {
        Self { id, is_on: false }
    }

    #[allow(unused)]
    pub fn get_report(&self) -> String {
        let mut rng = rand::thread_rng();
        let power = match self.is_on {
            true => rng.gen::<f64>(),
            false => 0.0,
        };
        format!(
            "type: socket, id: {}, is_on: {}, power: {power:.2}",
            self.id, self.is_on
        )
    }
}
