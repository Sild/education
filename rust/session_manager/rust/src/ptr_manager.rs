use crate::{cur_ts, new_session_id, SessionManager};
use std::collections::HashMap;

struct SessionData {
    user_id: u64,
    last_access: u32,
    session_id: String,
}

struct ListNode {
    prev: *mut ListNode,
    next: *mut ListNode,
    data: SessionData,
}

pub struct PtrManager {
    sessions: HashMap<String, ListNode>,
    oldest: *mut ListNode,
    newest: *mut ListNode,
}

impl PtrManager {
    pub fn new() -> Self {
        Self {
            sessions: HashMap::new(),
            oldest: std::ptr::null_mut(),
            newest: std::ptr::null_mut(),
        }
    }
}

impl SessionManager for PtrManager {
    fn print_nodes(&self) {
        let mut view = String::from("nodes: ");

        let mut cur_node = self.oldest;
        while !cur_node.is_null() {
            unsafe {
                view.push_str(&format!("{}->", (*cur_node).data.user_id));
                cur_node = (*cur_node).next;
            }
        }

        view.push_str("; rev_nodes: ");
        let mut cur_node = self.newest;
        while !cur_node.is_null() {
            unsafe {
                view.push_str(&format!("{}->", (*cur_node).data.user_id));
                cur_node = (*cur_node).prev;
            }
        }
        println!("{}", view);
    }
    fn create_session(&mut self, user_id: u64) -> String {
        let session_id = new_session_id();

        let node = ListNode {
            prev: self.newest,
            next: std::ptr::null_mut(),
            data: SessionData {
                user_id,
                last_access: cur_ts(),
                session_id: session_id.clone(),
            },
        };
        self.sessions.insert(session_id.clone(), node);
        let node_ptr = self.sessions.get_mut(&session_id).unwrap();

        if self.oldest.is_null() {
            self.newest = node_ptr;
            self.oldest = node_ptr;
            return session_id;
        }
        unsafe {
            (*self.newest).next = node_ptr;
        }
        self.newest = node_ptr;
        session_id
    }

    fn validate_session(&mut self, session_id: &str) -> Option<u64> {
        let node = match self.sessions.get_mut(session_id) {
            Some(node) => node,
            None => return None,
        };
        node.data.last_access = cur_ts();

        if node.next.is_null() {
            return Some(node.data.user_id);
        }

        if node.prev.is_null() {
            unsafe {
                node.prev = self.newest;
                self.oldest = node.next;
                (*self.oldest).prev = std::ptr::null_mut();
            }
        } else {
            unsafe {
                (*node.prev).next = node.next;
                (*node.next).prev = node.prev;
                node.prev = self.newest;
            }
        }

        unsafe { (*self.newest).next = node; }
        node.next = std::ptr::null_mut();
        self.newest = node;
        return Some(node.data.user_id);

    }

    fn cleanup(&mut self) {
        let cur_ts = cur_ts();
        let mut cur_node = self.oldest;
        unsafe {
            while !cur_node.is_null() && (*cur_node).data.last_access + 600 < cur_ts {
                self.sessions.remove(&(*cur_node).data.session_id);
                cur_node = (*cur_node).next;
            }
        }
    }
}
