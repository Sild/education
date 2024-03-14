#![feature(test)]

mod naive_manager;
mod ptr_manager;

use rand::{distributions::Alphanumeric, Rng};
use std::time::{SystemTime, UNIX_EPOCH};
use crate::naive_manager::NaiveManager;
use crate::ptr_manager::PtrManager;
extern crate test;



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
    fn format_nodes(&self)->String;
    fn print_nodes(&self) {
        println!("{}", self.format_nodes());
    }
    fn create_session(&mut self, user_id: u64) -> String;
    fn validate_session(&mut self, session_id: &str) -> Option<u64>;
    fn cleanup(&mut self);
}

fn test_correctness<T>(sm: &mut T) where T: SessionManager {
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
    test_correctness(&mut naive_sm);
    println!("{}", "PtrManager:");
    let mut ptr_sm = PtrManager::new();
    test_correctness(&mut ptr_sm);
}

#[cfg(test)]
mod tests {
    use super::*;
    use test::Bencher;

    #[test]
    fn test_correctness() {
        let mut sm_pool = Vec::<Box<dyn SessionManager>>::new();
        sm_pool.push(Box::new(NaiveManager::default()));
        sm_pool.push(Box::new(PtrManager::new()));
        for mut sm in sm_pool {
            // add
            let s1 = sm.create_session(1);
            assert_eq!(sm.format_nodes(), String::from("nodes: 1->; rev_nodes: 1->"));
            let s2 = sm.create_session(2);
            assert_eq!(sm.format_nodes(), String::from("nodes: 1->2->; rev_nodes: 2->1->"));
            let s3 = sm.create_session(3);
            assert_eq!(sm.format_nodes(), String::from("nodes: 1->2->3->; rev_nodes: 3->2->1->"));

            // validate
            let _ =  sm.validate_session(s1.as_str());
            assert_eq!(sm.format_nodes(), String::from("nodes: 2->3->1->; rev_nodes: 1->3->2->"));
            let _ = sm.validate_session(s2.as_str());
            assert_eq!(sm.format_nodes(), String::from("nodes: 3->1->2->; rev_nodes: 2->1->3->"));
            let _ = sm.validate_session(s3.as_str());
            assert_eq!(sm.format_nodes(), String::from("nodes: 1->2->3->; rev_nodes: 3->2->1->"));
            let _ = sm.validate_session(s2.as_str());
            assert_eq!(sm.format_nodes(), String::from("nodes: 1->3->2->; rev_nodes: 2->3->1->"));
            let _ = sm.validate_session(s2.as_str());
            assert_eq!(sm.format_nodes(), String::from("nodes: 1->3->2->; rev_nodes: 2->3->1->"));
        }
    }

    #[bench]
    fn bench_add_two(b: &mut Bencher) {
        // b.iter(|| add_two(2));
    }
}
