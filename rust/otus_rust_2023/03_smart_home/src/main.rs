use std::collections::{HashMap, HashSet};

use std::io::{Error, ErrorKind};
use std::ops::Index;
use rand::Rng;

const MAX_ROOMS: i32 = 4;
const MAX_SOCKET_PER_ROOM: i32 = 4;
const MAX_THERM_PER_ROOM: i32 = 4;

struct SmartHouse {
    rooms: HashSet<String>, // fail to keep &str as key in sockets ¯\_(ツ)_/¯
    sockets: HashMap<String, Vec<SmartSocket>>,
    thermometers: HashMap<String, Vec<SmartThermometer>>,
}

impl SmartHouse {
    fn new() -> Self {
        let mut rng = rand::thread_rng();
        let rooms_cnt = rng.gen_range(1..=MAX_ROOMS);
        let rooms = HashSet::from_iter((0..rooms_cnt).map(|i| (format!("Room_{}", i))));
        let mut sockets = HashMap::new();
        let mut thermometers = HashMap::new();

        for v in rooms.iter() {
            let sock_cnt = rng.gen_range(1..=MAX_SOCKET_PER_ROOM);
            let room_sockets = Vec::from_iter(
                (0..sock_cnt).map(|i| (
                    SmartSocket::new(format!("socket_{}", i))
                ))
            );
            sockets.insert(v.clone(), room_sockets);

            let therm_cnt = rng.gen_range(1..=MAX_THERM_PER_ROOM);
            let therms = Vec::from_iter(
                (0..therm_cnt).map(|i| (
                    SmartThermometer::new(format!("termo_{}", i))
                ))
            );
            thermometers.insert(v.clone(), therms);
        }
        SmartHouse {
            rooms,
            sockets,
            thermometers,
        }
    }

    fn get_rooms(&self) -> Vec<&str> {
        Vec::from_iter(self.rooms.iter().map(|r| r.as_str()))
    }

    fn devices(&self, room: &str) -> Result<Vec<String>, Error> {
        if !self.rooms.contains(room) {
            return Err(Error::new(ErrorKind::InvalidInput, "Room not found"));
        };
        let mut res = Vec::from_iter(self.sockets.index(room).iter().map(|x| format!("divice: {}, status: {:?}", x.id.as_str(), x.is_on)));
        res.append(&mut Vec::from_iter(self.thermometers.index(room).iter().map(|x| format!("divice: {}, status: {:?}", x.id.as_str(), x.is_on))));
        Ok(res)
    }

    fn create_report<T>(&self, provider: &T) -> String
        where T: DeviceInfoProvider {
        provider.print_info()
    }
}

trait DeviceInfoProvider {
    fn print_info(&self) -> String;
    // todo: метод, возвращающий состояние устройства по имени комнаты и имени устройства
}

// ***** Пример использования библиотеки умный дом:

// Пользовательские устройства:

struct SmartSocket {
    id: String,
    is_on: Option<bool>

}
impl SmartSocket {
    fn new(id: String) -> Self {
        Self {
            id,
            is_on: None,
        }
    }
}

struct SmartThermometer {
    id: String,
    is_on: Option<bool>

}
impl SmartThermometer {
    fn new(id: String) -> Self {
        Self {
            id,
            is_on: None,
        }
    }
}

// Пользовательские поставщики информации об устройствах.
// Могут как хранить устройства, так и заимствывать.
struct OwningDeviceInfoProvider {
    socket: SmartSocket,
}

impl DeviceInfoProvider for OwningDeviceInfoProvider {
    fn print_info(&self) -> String {
        format!("DeviceInfoProvider: socket_id: {}, socket_status: {:?}", self.socket.id, self.socket.is_on)
    }
}
struct BorrowingDeviceInfoProvider<'a, 'b> {
    socket: &'a SmartSocket,
    thermo: &'b SmartThermometer,
}

impl DeviceInfoProvider for BorrowingDeviceInfoProvider<'_, '_> {
    fn print_info(&self) -> String {
        format!("BorrowingDeviceInfoProvider: socket_id: {}, socket_status: {:?}, therm_id: {}, term_status: {:?}", self.socket.id, self.socket.is_on, self.thermo.id, self.thermo.is_on)
    }
}

fn main() {
    // Инициализация устройств
    let socket1 = SmartSocket::new("4".into());
    let socket2 = SmartSocket::new("5".into());
    let thermo = SmartThermometer::new("6".into());

    // Инициализация дома
    let house = SmartHouse::new();


    // Строим отчёт с использованием `OwningDeviceInfoProvider`.
    let info_provider_1 = OwningDeviceInfoProvider {
        socket: socket1,
    };
    let report1 = house.create_report(&info_provider_1);

    // Строим отчёт с использованием `BorrowingDeviceInfoProvider`.
    let info_provider_2 = BorrowingDeviceInfoProvider {
        socket: &socket2,
        thermo: &thermo,
    };
    // todo: после добавления обобщённого аргумента в метод, расскоментировать передачу параметра
    let report2 = house.create_report(&info_provider_2);

    // Выводим отчёты на экран:
    println!("Report #1: {report1}");
    println!("Report #2: {report2}");

    for room in house.get_rooms() {
        println!("Devices in room {}: {}", room, house.devices(room).unwrap().join(", "));
    }
}