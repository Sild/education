mod naive_manager;
mod ptr_manager;

use rand::{distributions::Alphanumeric, Rng};
use std::time::{SystemTime, UNIX_EPOCH};
use crate::naive_manager::NaiveManager;
use crate::ptr_manager::PtrManager;

fn new_session_id() -> String {
    rand::thread_rng()
        .sample_iter(&Alphanumeric)
        .take(32)
        .map(char::from)
        .collect::<String>()
}

fn cur_ts() -> u32 {
    let start = SystemTime::now();
    start.duration_since(UNIX_EPOCH)
        .expect("Time went backwards").as_secs() as u32
}

trait SessionManager {
    fn print_nodes(&self);
    fn create_session(&mut self, user_id: u64) -> String;
    fn validate_session(&mut self, session_id: &str) -> Option<u64>;
    fn cleanup(&mut self);
}

fn test<T>(sm: &mut T) where T: SessionManager {
    let s1 = sm.create_session(1);
    sm.print_nodes();

    let s2 = sm.create_session(2);
    sm.print_nodes();

    let s3 = sm.create_session(3);

    sm.print_nodes();
    let _ =  sm.validate_session(s1.as_str());
    sm.print_nodes();
    let _ = sm.validate_session(s2.as_str());
    sm.print_nodes();
    let _ = sm.validate_session(s3.as_str());
    sm.print_nodes();
    let _ = sm.validate_session(s2.as_str());
    sm.print_nodes();
    let _ = sm.validate_session(s2.as_str());
    sm.print_nodes();
}



fn main() {
    println!("{}", "NaiveManager:");
    let mut naive_sm = NaiveManager::default();
    test(&mut naive_sm);
    println!("{}", "PtrManager:");
    let mut ptr_sm = PtrManager::new();
    test(&mut ptr_sm);
}
