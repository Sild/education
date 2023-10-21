use crate::devices::socket::Socket;
use crate::devices::thermo::Thermo;
use crate::devices::IdType;
use crate::house::traits::{DeviceVisitor, SmartDevice};
use std::any::Any;
use std::collections::{HashMap, HashSet};

#[derive(Default)]
pub struct ReportVisitor {
    pub reports: HashMap<String, HashMap<IdType, String>>,
    pub errors: Vec<String>,
}

impl DeviceVisitor for ReportVisitor {
    fn visit(&mut self, room_id: &str, any_device: &dyn Any) {
        let report_entry = self.reports.entry(room_id.to_string()).or_default();
        if let Some(device) = any_device.downcast_ref::<Socket>() {
            report_entry.insert(device.get_id().into(), device.get_report());
            return;
        }
        if let Some(device) = any_device.downcast_ref::<Thermo>() {
            report_entry.insert(device.get_id().into(), device.get_report());
            return;
        }
        self.errors
            .push(format!("Fail to get device_type in room {}", room_id))
    }
}

impl ReportVisitor {
    pub fn print_report(&self) {
        for (room_id, reports) in self.reports.iter() {
            println!("report for room_id: {}", room_id);
            for (_, rep) in reports.iter() {
                println!("{}", rep)
            }
            println!();
        }
        if self.errors.is_empty() {
            println!("errors: no errors")
        } else {
            println!("errors:");
            for err in self.errors.iter() {
                println!("{err}")
            }
        }
    }
}

#[derive(Default)]
pub struct SwitchStatusVisitor {
    device_ids: HashSet<String>,
    pub errors: Vec<String>,
}

impl SwitchStatusVisitor {
    pub fn new(device_ids_src: Vec<&str>) -> Self {
        let mut device_ids = HashSet::new();
        for d in device_ids_src.iter() {
            device_ids.insert(d.to_string());
        }
        SwitchStatusVisitor {
            device_ids,
            errors: Default::default(),
        }
    }
}

impl DeviceVisitor for SwitchStatusVisitor {
    fn visit_mut(&mut self, room_id: &str, any_device: &mut dyn Any) {
        if let Some(device) = any_device.downcast_mut::<Socket>() {
            if self.device_ids.is_empty() || self.device_ids.contains(device.get_id()) {
                device.is_on = !device.is_on;
            }
            return;
        }
        if let Some(device) = any_device.downcast_mut::<Thermo>() {
            if self.device_ids.is_empty() || self.device_ids.contains(device.get_id()) {
                device.is_on = !device.is_on;
            }
            return;
        }
        self.errors
            .push(format!("Fail to get device_type in room {}", room_id))
    }
}
