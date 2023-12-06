pub trait MyTrait {
    fn foo(&self) -> i32;
}

#[derive(Default)]
pub struct Pod {}

impl MyTrait for Pod {
    fn foo(&self) -> i32 {
        42
    }
}

pub mod dec_static {
    use crate::decorator::MyTrait;

    pub struct LogDecorator<T> {
        inner: T,
    }

    impl<T: MyTrait> LogDecorator<T> {
        pub fn new(inner: T) -> Self {
            Self { inner }
        }
    }

    impl<T: MyTrait> MyTrait for LogDecorator<T> {
        fn foo(&self) -> i32 {
            println!("method foo called from static");
            self.inner.foo()
        }
    }
}

pub mod dec_dyn {
    use crate::decorator::MyTrait;

    pub struct LogDecorator {
        inner: Box<dyn MyTrait>,
    }

    impl LogDecorator {
        pub fn new(inner: Box<dyn MyTrait>) -> Self {
            Self { inner }
        }
    }

    impl MyTrait for LogDecorator {
        fn foo(&self) -> i32 {
            println!("method foo called from dynamic");
            self.inner.foo()
        }
    }
}
