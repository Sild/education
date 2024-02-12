use rand::Rng;

pub struct Socket {
    id: String,
    pub is_on: bool,
}

impl Socket {
    pub fn new(id: &str) -> Self {
        Self {
            id: id.to_string(),
            is_on: false,
        }
    }

    pub fn get_report(&self) -> String {
        let mut rng = rand::thread_rng();
        let power = match self.is_on {
            true => rng.gen::<f64>(),
            false => 0.0,
        };
        format!(
            "type: socket_lib, id: {}, is_on: {}, power: {power:.2}",
            self.id, self.is_on
        )
    }
}

static mut SOCKET: Option<Socket> = None;

unsafe fn init_socket() {
    if SOCKET.is_none() {
        SOCKET = Some(Socket::new("lib-socket_lib"));
    }
}

#[no_mangle]
pub extern "C" fn switch_socket_status() {
    unsafe {
        init_socket();
        SOCKET.as_mut().unwrap().is_on = !SOCKET.as_ref().unwrap().is_on;
    }
}

#[no_mangle]
pub extern "C" fn print_socket_report() {
    unsafe {
        init_socket();
        println!("{}", SOCKET.as_ref().unwrap().get_report());
    }
}
