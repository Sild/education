#[cfg(test)]
use smart_home;
use smart_home::devices::socket::Socket;
use smart_home::devices::thermo::Thermometer;
use smart_home::house::traits::SmartDevice;

#[test]
fn test_device_with_id_trait() {
    let id = "test1";
    let sock = Socket::new(id.to_string());
    assert_eq!(sock.get_id(), id);
    let therm = Thermometer::new(id.to_string());
    assert_eq!(therm.get_id(), id);
}

#[test]
fn test_socket_get_report() {
    let id = "test1";
    let sock = Socket::new(id.to_string());
    assert!(sock.get_report().contains("socket"));
    let mut therm = Thermometer::new(id.to_string());
    assert!(therm.get_report().contains("thermometer"));
    assert!(therm.get_report().contains("is_on: false"));
    therm.is_on = true;
    assert!(therm.get_report().contains("is_on: true"));
    assert!(therm.get_report().contains("cur_temp: "));
}

#[test]
fn test_thermo_get_report() {
    let id = "test1";
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
