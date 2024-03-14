use std::cell::RefCell;
use std::collections::HashMap;
use std::rc::Rc;
use crate::{cur_ts, new_session_id, SessionManager};

struct SessionData {
    user_id: u64,
    last_access: u32,
    session_id: String
}

type ListNodePtr = Rc<RefCell<ListNode>>;

struct ListNode {
    prev: Option<ListNodePtr>,
    next: Option<ListNodePtr>,
    data: SessionData
}

#[derive(Default)]
pub struct NaiveManager {
    sessions: HashMap<String, ListNodePtr>,
    oldest: Option<ListNodePtr>,
    newest: Option<ListNodePtr>,
}

impl SessionManager for NaiveManager {
    fn print_nodes(&self) {
        let mut view = String::from("nodes: ");

        let mut node = self.oldest.clone();
        while node.is_some() {
            view.push_str(&format!("{}->", node.as_ref().unwrap().borrow().data.user_id));
            let tmp = node.as_ref().unwrap().borrow().next.clone();
            node = tmp;
        }

        view.push_str("; rev_nodes: ");
        let mut node = self.newest.clone();
        while node.is_some() {
            view.push_str(&format!("{}->", node.as_ref().unwrap().borrow().data.user_id));
            let tmp = node.as_ref().unwrap().borrow().prev.clone();
            node = tmp;
        }
        println!("{}", view);
    }
    fn create_session(&mut self, user_id: u64) -> String {
        let session_id = new_session_id();
        let node = Rc::new(RefCell::new(ListNode{
            prev: match self.newest {
                Some(ref node) => Some(node.clone()),
                None => None
            },
            next: None,
            data: SessionData {
                user_id,
                last_access: cur_ts(),
                session_id: session_id.clone(),
            }
        }));
        self.sessions.insert(session_id.clone(), node.clone());

        if self.oldest.is_none() {
            self.oldest = Some(node.clone());
            self.newest = Some(node.clone());
            return session_id
        }

        self.newest.as_ref().unwrap().borrow_mut().next = Some(node.clone());
        self.newest = Some(node.clone());
        session_id
    }

    fn validate_session(&mut self, session_id: &str) -> Option<u64> {
        let node = match self.sessions.get_mut(session_id) {
            Some(node) => node,
            None => return None
        };

        node.borrow_mut().data.last_access = cur_ts();
        if node.borrow().next.is_none() {
            return Some(node.borrow().data.user_id);
        }
        if node.borrow().prev.is_none() {
            node.borrow_mut().prev = self.newest.clone();
            self.oldest = node.borrow().next.clone();
            self.oldest.as_ref().unwrap().borrow_mut().prev = None;
        } else {
            node.borrow().prev.as_ref().unwrap().borrow_mut().next = node.borrow().next.clone();
            node.borrow().next.as_ref().unwrap().borrow_mut().prev = node.borrow().prev.clone();
            node.borrow_mut().prev = self.newest.clone();
        }

        self.newest.as_ref().unwrap().borrow_mut().next = Some(node.clone());
        self.newest = Some(node.clone());
        node.borrow_mut().next = None;
        Some(node.borrow().data.user_id)
    }

    fn cleanup(&mut self) {
        let cur_ts = cur_ts();
        let mut cur_node = self.oldest.clone();
        while cur_node.is_some() && cur_node.as_ref().unwrap().borrow().data.last_access + 600 < cur_ts {
            self.sessions.remove(&cur_node.as_ref().unwrap().borrow().data.session_id);
            cur_node = cur_node.unwrap().borrow().next.clone();
        }
    }
}