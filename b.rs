//extern crate green;
//extern crate rustuv;
extern crate time;

/*
#[start]
fn start(argc: int, argv: **u8) -> int {
    green::start(argc, argv, green::basic::event_loop, main)
}
*/

struct Point {
    x: int,
    y: int,
}

impl  <'r>Index<int,& 'r Point> for Point {
    fn index(& 'r self, index: &int) ->  & 'r  Point { println!("index={}" , index);self }

}

fn main() {
    let t = time::now();
    let st = time::strftime(&"%Y-%m-%d", &t);
    println!("{}" , st)
    let mut  p1 = Point{x: 1, y: 2,};
    p1 = Point{x: 444, y: 444,};
    println!("p1[2].x={}" , p1 [ 2 ] . x);
}
