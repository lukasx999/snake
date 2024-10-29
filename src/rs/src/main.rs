

enum Direction {
    Right,
    Left,
    Up,
    Down,
}

struct Vector<T> {
    x: T,
    y: T,
}

struct SnakeNode {
    position:  Vector<u32>,
    direction: Direction,
    next:      Option<Box<SnakeNode>>,
    prev:      Option<Box<SnakeNode>>,
}

impl SnakeNode {
    fn new(&mut self) {
    }
}

struct Game {
    snake_head: Box<SnakeNode>,
    width:  u32,
    height: u32,
}


impl Game {
    pub fn new(width: u32, height: u32) -> Game {

        Game {
            width,
            height,
            snake_head: Box::new(
                SnakeNode {
                    position: Vector {
                        x: width  / 2,
                        y: height / 2,
                    },
                    direction: Direction::Left,
                    next:      Option::None,
                    prev:      Option::None,
                }
            ),
        }


    }

    pub fn snake_grow(&mut self) -> () {

        // let mut current: &Box<SnakeNode> = &self.snake_head;

        let mut current: &Option<Box<SnakeNode>> = &self.snake_head.next;
        while current != Option::None {

            current = &current.unwrap().next;

        }


    }



}




fn main() {

    let game = Game::new(50, 50);




}
