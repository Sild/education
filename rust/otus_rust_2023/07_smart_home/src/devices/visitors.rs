use crate::devices::socket::Socket;
use crate::devices::thermo::Thermo;
use crate::house::traits::{DeviceVisitor, SmartDevice};
use std::any::Any;
use std::collections::HashMap;
use crate::devices::IdType;

#[derive(Default)]
pub struct ReportVisitor {
    pub reports: HashMap<String, HashMap<IdType, String>>,
    errors: Vec<String>,
}

impl DeviceVisitor for ReportVisitor {
    fn visit(&mut self, room_id: &str, any_device: &dyn Any) {
        let report_entry = self
            .reports
            .entry(room_id.to_string()).or_insert(Default::default());
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
pub struct TurnOnVisitor {
    errors: Vec<String>,
}

impl DeviceVisitor for TurnOnVisitor {
    fn visit_mut(&mut self, room_id: &str, any_device: &mut dyn Any) {
        if let Some(device) = any_device.downcast_mut::<Socket>() {
            device.is_on = true;
            return;
        }
        if let Some(device) = any_device.downcast_mut::<Thermo>() {
            device.is_on = true;
            return;
        }
        self.errors
            .push(format!("Fail to get device_type in room {}", room_id))
    }
}
