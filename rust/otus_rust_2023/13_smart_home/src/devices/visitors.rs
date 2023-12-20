use crate::devices::IdType;
use crate::house::traits::DeviceVisitor;
use std::collections::{HashMap, HashSet};

use super::Device;

#[derive(Default)]
pub struct ReportVisitor {
    pub reports: HashMap<String, HashMap<IdType, String>>,
    pub errors: Vec<String>,
}

impl DeviceVisitor<Device> for ReportVisitor {
    fn visit(&mut self, room_id: &str, device: &Device) {
        let report_entry = self.reports.entry(room_id.to_string()).or_default();
        match device {
            Device::Socket(d) => report_entry.insert(d.get_id().into(), d.get_report()),
            Device::Thermo(d) => report_entry.insert(d.get_id().into(), d.get_report()),
        };
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

impl DeviceVisitor<Device> for SwitchStatusVisitor {
    fn visit_mut(&mut self, _room_id: &str, device: &mut Device) {
        match device {
            Device::Socket(d) => {
                if self.device_ids.is_empty() || self.device_ids.contains(d.get_id()) {
                    d.is_on = !d.is_on;
                }
            }
            Device::Thermo(d) => {
                if self.device_ids.is_empty() || self.device_ids.contains(d.get_id()) {
                    d.is_on = !d.is_on;
                }
            }
        }
    }
}
