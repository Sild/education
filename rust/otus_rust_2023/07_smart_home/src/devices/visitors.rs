use crate::house::traits::DeviceVisitor;
use std::any::Any;
use std::collections::HashMap;
use crate::devices::Type;

#[derive(Default)]
pub struct ReportVisitor {
    reports: HashMap<String, Vec<String>>,
    errors: Vec<String>,
}

impl DeviceVisitor<Type> for ReportVisitor {
    fn visit(&mut self, room_id: &str, device: &Type) {
        let default = Vec::new();
        let report_entry = self
            .reports
            .entry(room_id.to_string())
            .or_insert_with(|| default);
        match device {
            Type::Socket(d) => report_entry.push(d.get_report()),
            Type::Thermo(d) => report_entry.push(d.get_report()),
            _ => self.errors.push(format!("unexpected device_type in room {}", room_id))
        }

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

impl DeviceVisitor<Type> for TurnOnVisitor {
    fn visit_mut(&mut self, room_id: &str, any_device: &mut Type) {
        // if let Some(device) = any_device.downcast_mut::<Socket>() {
        //     device.is_on = true;
        //     return;
        // }
        // if let Some(device) = any_device.downcast_mut::<Thermometer>() {
        //     device.is_on = true;
        //     return;
        // }
        // self.errors
        //     .push(format!("Fail to get device_type in room {}", room_id))
    }
}
