use std::collections::HashMap;
use std::hash::Hash;
use std::io::{Error, ErrorKind};
use rand;
use rand::Rng;


const MAX_ROOMS: i32 = 4;
const MAX_SOCKET_PER_ROOM: i32 = 4;
const MAX_THERM_PER_ROOM: i32 = 4;

struct SmartHouse {
    rooms: HashMap<String, i32>, // fail to keep &str as key in sockets ¯\_(ツ)_/¯
    sockets: HashMap<i32, Vec<SmartSocket>>,
    thermometers: HashMap<i32, Vec<SmartThermometer>>,
}

impl<'a> SmartHouse {
    fn new() -> Self {
        let mut rng = rand::thread_rng();
        let rooms_cnt = rng.gen_range(1..=MAX_ROOMS);
        let rooms = HashMap::from_iter((0..rooms_cnt).map(|i| (format!("Room_{}", i), i)));
        let mut sockets = HashMap::new();
        let mut thermometers = HashMap::new();

        for (_, v) in rooms.iter() {
            let sock_cnt = rng.gen_range(1..=MAX_SOCKET_PER_ROOM);
            let room_sockets = Vec::from_iter(
                (0..sock_cnt).map(|i| (
                    SmartSocket::new(format!("socket_{}", i))
                ))
            );
            sockets.insert(*v, room_sockets);

            let therm_cnt = rng.gen_range(1..=MAX_THERM_PER_ROOM);
            let therms = Vec::from_iter(
                (0..therm_cnt).map(|i| (
                    SmartThermometer::new(format!("termo_{}", i))
                ))
            );
            thermometers.insert(*v, therms);
        }
        SmartHouse {
            rooms,
            sockets,
            thermometers,
        }
    }

    fn get_rooms(&self) -> Vec<String> {
        // It's awful. Must find a way to keep Vector instead of HashMap.
        Vec::from_iter(self.rooms.keys().cloned())
    }

    fn devices(&self, room: &str) -> Result<Vec<&str>, Error> {
        let index = match self.rooms.get(room.into()) {
            Some(x) => x,
            None => return Err(Error::new(ErrorKind::InvalidInput, "Room not found")),
        };
        let mut res = Vec::from_iter(self.sockets[index].iter().map(|x| x.id.as_str()));
        res.append(&mut Vec::from_iter(self.thermometers[index].iter().map(|x| x.id.as_str())));
        Ok(res)
    }

    fn create_report<T>(&self, provider: T) -> String
        where T: DeviceInfoProvider {
        todo!("перебор комнат и устройств в них для составления отчёта")
    }
}

trait DeviceInfoProvider {
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
struct BorrowingDeviceInfoProvider<'a, 'b> {
    socket: &'a SmartSocket,
    thermo: &'b SmartThermometer,
}


// todo: реализация трейта `DeviceInfoProvider` для поставщиков информации

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
    // todo: после добавления обобщённого аргумента в метод, расскоментировать передачу параметра
    // let report1 = house.create_report(/* &info_provider_1 */);

    // Строим отчёт с использованием `BorrowingDeviceInfoProvider`.
    let info_provider_2 = BorrowingDeviceInfoProvider {
        socket: &socket2,
        thermo: &thermo,
    };
    // todo: после добавления обобщённого аргумента в метод, расскоментировать передачу параметра
    // let report2 = house.create_report(/* &info_provider_2 */);

    // Выводим отчёты на экран:
    // println!("Report #1: {report1}");
    // println!("Report #2: {report2}");
}