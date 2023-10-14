use crate::devices::socket::Socket;
use crate::devices::thermo::Thermometer;
use crate::house::traits::DeviceVisitor;
use std::any::Any;
use std::collections::HashMap;

#[derive(Default)]
pub struct ReportVisitor {
    reports: HashMap<String, Vec<String>>,
    errors: Vec<String>,
}

impl DeviceVisitor for ReportVisitor {
    fn visit(&mut self, room_id: &str, any_device: &dyn Any) {
        let default = Vec::new();
        let report_entry = self
            .reports
            .entry(room_id.to_string())
            .or_insert_with(|| default);
        if let Some(device) = any_device.downcast_ref::<Socket>() {
            report_entry.push(device.get_report());
            return;
        }
        if let Some(device) = any_device.downcast_ref::<Thermometer>() {
            report_entry.push(device.get_report());
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
            for rep in reports {
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
        if let Some(device) = any_device.downcast_mut::<Thermometer>() {
            device.is_on = true;
            return;
        }
        self.errors
            .push(format!("Fail to get device_type in room {}", room_id))
    }
}
