mod newtype {
    use std::fmt::Display;

    pub struct MyString(pub String);

    impl Display for MyString {
        fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
            let string: String = self
                .0
                .chars()
                .map(|x| match x {
                    '_' => '-',
                    'a'..='z' => '*',
                    _ => x,
                })
                .collect();
            write!(f, "{}", string)
        }
    }
}

mod raii {
    pub struct MyString(pub String);

    impl Drop for MyString {
        fn drop(&mut self) {
            println!("raii::MyString with data `{}` dropped!", self.0);
        }
    }
}

mod builder {
    pub struct StringBuilder {
        data: String,
    }

    impl StringBuilder {
        pub fn new(data: &str) -> StringBuilder {
            StringBuilder {
                data: String::from(data),
            }
        }

        pub fn with_preffix(mut self, data: &str) -> StringBuilder {
            self.data = data.to_owned() + self.data.as_str();
            self
        }

        pub fn with_suffix(mut self, data: &str) -> StringBuilder {
            self.data += data;
            self
        }

        pub fn build(self) -> String {
            self.data
        }
    }
}

fn main() {
    let a = newtype::MyString(String::from("my_fun_string"));
    println!("newtype::MyString: {}", a);

    raii::MyString(String::from("my_fun_string"));

    let c = builder::StringBuilder::new("my_fun_string")
        .with_preffix("prefix_")
        .with_suffix("_suffix")
        .build();
    println!("builder::String: {}", c);
}
