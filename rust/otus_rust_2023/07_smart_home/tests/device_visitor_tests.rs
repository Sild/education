use smart_home;
use smart_home::devices::visitors::{ReportVisitor, SwitchStatusVisitor};
use smart_home::house::traits::{DeviceVisitor};
#[cfg(test)]
use std::any::Any;
use smart_home::devices::socket::Socket;

#[test]
fn test_switch_status() {
    let mut report_visitor = ReportVisitor::default();
    let mut socket: Box<dyn Any> = Box::new(Socket::new("s1".to_string()));
    report_visitor.visit("test_room", &(*socket));
    assert!(report_visitor.reports.get("test_room").unwrap().get("s1").unwrap().contains("is_on: false"));

    let mut switch_visitor = SwitchStatusVisitor::default();
    switch_visitor.visit_mut("test_room", &mut (*socket));
    report_visitor.visit("test_room", &(*socket));
    assert!(report_visitor.reports.get("test_room").unwrap().get("s1").unwrap().contains("is_on: true"));
    switch_visitor.visit_mut("test_room", &mut (*socket));
    report_visitor.visit("test_room", &(*socket));
    assert!(report_visitor.reports.get("test_room").unwrap().get("s1").unwrap().contains("is_on: false"));
}
