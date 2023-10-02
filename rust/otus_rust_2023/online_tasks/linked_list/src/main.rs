// Полезные методы:
// - https://doc.rust-lang.org/stable/std/option/enum.Option.html#method.as_mut
// - https://doc.rust-lang.org/stable/std/option/enum.Option.html#method.as_ref
// - https://doc.rust-lang.org/stable/std/option/enum.Option.html#method.take
// - https://doc.rust-lang.org/stable/std/option/enum.Option.html#method.map

pub struct List<T> {
    head: Link<T>,
}

// Что будет, если убрать здесь Box? Попробуйте.
type Link<T> = Option<Box<Node<T>>>;

struct Node<T> {
    elem: T,
    next: Link<T>,
}

impl<T> List<T> {
    pub fn new() -> Self {
        List { head: None}
    }

    pub fn push(&mut self, elem: T) {
        let new_node = Some(Box::new(Node{elem, next: None}));
        match self.head.as_mut() {
            Some(mut h) => {
                while h.next.is_some() {
                    h = h.next.as_mut().unwrap();
                }
                h.next = new_node
            },
            None => self.head = new_node,
        }
    }

    pub fn pop(&mut self) -> Option<T> {
        if self.head.is_none() {
            return None;
        }
        if self.head.as_mut().unwrap().next.is_none() {
            let res = self.head.take();
            return Some(res.unwrap().elem);
        }

        let mut prev = &mut self.head;
        let mut cur = &mut prev.as_mut().unwrap().next;
        while cur.as_ref().unwrap().next.is_some() {
            prev = cur;
            cur = &mut prev.as_mut().unwrap().next;
        }
        Some(cur.take().unwrap().elem)
    }

    pub fn peek(&self) -> Option<&T> {
        if self.head.is_none() {
            return None;
        }
        let mut cur = &self.head;
        while cur.as_ref().unwrap().next.is_some() {
            cur = &cur.as_ref().unwrap().next;
        }
        let last = cur;
        return Some(&(last.as_ref().unwrap().elem))
    }

    pub fn peek_mut(&mut self) -> Option<&mut T> {
        if self.head.is_none() {
            return None;
        }
        let mut cur = &mut self.head;
        while cur.as_mut().unwrap().next.is_some() {
            cur = &mut cur.as_mut().unwrap().next;
        }
        let last = cur;
        return Some(&mut last.as_mut().unwrap().elem)
    }

}

impl<T> Drop for List<T> {
    fn drop(&mut self) {
    }
}

#[cfg(test)]
mod test {
    use super::List;

    #[test]
    fn basics() {
        let mut list = List::new();

        // Check empty list behaves right
        assert_eq!(list.pop(), None);

        // Populate list
        list.push(1);
        list.push(2);
        list.push(3);

        // Check normal removal
        assert_eq!(list.pop(), Some(3));
        assert_eq!(list.pop(), Some(2));

        // Push some more just to make sure nothing's corrupted
        list.push(4);
        list.push(5);

        // Check normal removal
        assert_eq!(list.pop(), Some(5));
        assert_eq!(list.pop(), Some(4));

        // Check exhaustion
        assert_eq!(list.pop(), Some(1));
        assert_eq!(list.pop(), None);
    }

    #[test]
    fn peek() {
        let mut list = List::new();
        assert_eq!(list.peek(), None);
        assert_eq!(list.peek_mut(), None);
        list.push(1); list.push(2); list.push(3);

        assert_eq!(list.peek(), Some(&3));
        assert_eq!(list.peek_mut(), Some(&mut 3));

        list.peek_mut().map(|value| {
            *value = 42
        });

        assert_eq!(list.peek(), Some(&42));
        assert_eq!(list.pop(), Some(42));
    }
}

fn main() {}
