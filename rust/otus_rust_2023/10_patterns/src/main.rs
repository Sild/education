mod decorator;

fn show_decorator() {
    let execute = |obj: &dyn decorator::DecTrait| {
        let res = obj.foo();
        println!("res={}", res);
    };
    let item = decorator::Pod::default();
    execute(&item);

    let decorator = decorator::LogDecorator::new(item);
    execute(&decorator);

}

fn main() {
    show_decorator();
}
