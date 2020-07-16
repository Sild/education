fn fibo(num: i32) -> i32 {
    return if num > 1 {
        fibo(num - 1) + fibo(num - 2)
    } else {
        num
    }
}

fn main() {
    println!("Hello, world!");
    let a = [3, 2, 1];
    let mut i = 0;
    while i < 3 {
        println!("val: {}", a[i]);
        i = i + 1;
    }

    for e in a.iter() {
        println!("also val: {}", e);
    }

    for num in (1..4).rev() {
        println!("rev num: {}", num)
    }

    print!("fibo0..49: ");
    for i in 0..50 {
        print!("{} ", fibo(i));
    }
}
