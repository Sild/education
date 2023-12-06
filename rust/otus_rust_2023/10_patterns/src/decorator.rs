pub trait DecTrait {
    fn foo(&self) -> i32;
}

pub struct LogDecorator<T> {
    inner: T,
}

impl<T: DecTrait> LogDecorator<T> {
    pub fn new(inner: T) -> Self {
        Self { inner }
    }
}

impl<T: DecTrait> DecTrait for LogDecorator<T> {
    fn foo(&self) -> i32 {
        println!("method foo called");
        self.inner.foo()
    }
}

#[derive(Default)]
pub struct Pod {

}

impl DecTrait for Pod {
    fn foo(&self) -> i32 {
        42
    }
}
