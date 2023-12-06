mod decorator;

fn run_decorators() {
    let execute = |obj: &dyn decorator::MyTrait| {
        let res = obj.foo();
        println!("res={}", res);
    };
    let item = decorator::Pod::default();
    execute(&item);

    let decorator = decorator::dec_static::LogDecorator::new(item);
    execute(&decorator);

    let item = decorator::Pod::default();
    let decorator = decorator::dec_dyn::LogDecorator::new(Box::new(item));
    execute(&decorator);
}

fn main() {
    run_decorators();
}
