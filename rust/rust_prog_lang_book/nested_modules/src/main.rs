pub mod a {
    pub mod series {
        pub mod of {
            pub fn nested_modules() {
            }
        }
    }
}

use a::series::of;

enum TrafficLight {
    Red,
    Yellow,
    Green,
}

use TrafficLight::{Red, Green};

fn main() {
    of::nested_modules();
    let red = Red;
    let yellow = TrafficLight::Yellow;
    {
        use TrafficLight::*;
        let green = Green;
    }
}
