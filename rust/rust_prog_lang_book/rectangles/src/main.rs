#[derive(Debug)]
struct Rect {
    len: u32,
    wei: u32,
}

impl Rect {
    fn area(&self) -> u32 {
        self.len * self.wei
    }

    fn squere(s: u32) -> Rect {
        Rect{len: s, wei: s}
    }
}

fn main() {
    let len1 = 50;
    let wei1 = 30;
    println!("area is: {}", area(len1, wei1));

    let rect = (30, 50);
    println!("area2 is: {}", area2(rect));

    let rect = Rect{len: 30, wei: 50};
    println!("area3 is: {}", area3(&rect));

    println!("rect: {:#?}", rect);

    println!("area4 is: {}", rect.area());

    println!("squere: {:?}", Rect::squere(4));

}

fn area2(dim: (u32, u32)) -> u32 {
    dim.0 * dim.1
}

fn area(len: u32, wei: u32) -> u32 {
    len * wei
}

fn area3(r: &Rect) -> u32 {
    r.len * r.wei
}
