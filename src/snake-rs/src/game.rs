

use raylib::color::Color;

#[derive(Clone, Debug, PartialEq)]
pub enum Direction {
    Right,
    Left,
    Up,
    Down,
}

#[derive(Clone, Debug, PartialEq)]
pub struct SnakeNode {
    pub x: i32,
    pub y: i32,
    pub direction: Direction,
}

impl SnakeNode {
    pub fn new(x: i32, y: i32, direction: Direction) -> Self {
        Self { x, y, direction }
    }
}



#[derive(Clone, Debug)]
pub enum CellType {
    Background,
    Border,
    Wrap,
    Snake,
    Apple,
    Empty,
}


#[derive(Clone, Debug)]
pub struct Drawable {
    pub type_: CellType,
    pub color: Color,
}


impl Drawable {
    pub fn new(type_: CellType, color: Color) -> Self {
        Self { type_, color }
    }
    pub const EMPTY: Self = Self {
        type_: CellType::Empty,
        color: Color::BLACK
    };
}



#[derive(Debug)]
pub struct Game {
    pub width:  u32,
    pub height: u32,
    pub canvas: Vec<Vec<Drawable>>,
    pub snake:  Vec<SnakeNode>,
}

impl Game {

    pub fn new(width: u32, height: u32, start_size: usize) -> Self {
        let mut s = Self {
            width,
            height,
            canvas: Vec::new(),
            snake:  Vec::new(),
        };

        // Create head snake node
        s.snake.push(SnakeNode::new(
            s.width  as i32 / 2,
            s.height as i32 / 2,
            Direction::Left,
        ));

        for _ in 0..start_size {
            s.snake_grow();
        }

        for _ in 0..s.width {
            s.canvas.push(vec![Drawable::EMPTY; s.height as usize]);
        }

        s

    }



    // Fill the canvas with bg cells
    fn canvas_clear(&mut self) -> () {

        let bg = Drawable::new(
            CellType::Background,
            Color::new(20, 20, 20, 255)
        );

        for x in 0..self.width as usize {
            for y in 0..self.height as usize {
                self.canvas[x][y] = bg.clone();
            }
        }

    }

    fn snake_update(&mut self) -> () {

        let snake_copy = self.snake.clone();

        for node in &mut self.snake {

            let current: usize = snake_copy.iter().position(
                |x| *x == *node
            ).unwrap();

            let before: usize = match current.checked_sub(1usize) {
                Some(value) => value,
                None        => 0usize,
            };

            let before: &SnakeNode = &snake_copy[before];

            match node.direction {
                Direction::Left  => node.x -= 1,
                Direction::Right => node.x += 1,
                Direction::Up    => node.y -= 1,
                Direction::Down  => node.y += 1,
            };

            node.direction = before.direction.clone();

        }

    }





    pub fn update(&mut self) -> () {

        self.canvas_clear();

        let border = Drawable::new(CellType::Border, Color::BLUE);

        // Border Up
        for x in 0..self.width as usize {
            self.canvas[x][0] = border.clone();
        }

        // Border Down
        for x in 0..self.width as usize {
            let index = self.height as usize - 1;
            self.canvas[x][index] = border.clone();
        }

        // Border Left
        for y in 0..self.height as usize {
            self.canvas[0][y] = border.clone();
        }

        // Border Right
        for y in 0..self.height as usize {
            let index = self.width as usize - 1;
            self.canvas[index][y] = border.clone();
        }


        // Put snake into canvas
        let snake = Drawable::new(CellType::Snake, Color::RED);
        for item in &self.snake {
            self.canvas[item.x as usize][item.y as usize] = snake.clone();
        }

        self.snake_update();

    }

    pub fn snake_move(&mut self, direction: Direction) -> () {
        self.snake[0].direction = direction;
    }

    pub fn snake_grow(&mut self) -> () {
        let last: &SnakeNode = self.snake.last().unwrap();

        self.snake.push(
            SnakeNode::new(
                match last.direction {
                    Direction::Left  => last.x + 1,
                    Direction::Right => last.x - 1,
                    _ => last.x,
                },
                match last.direction {
                    Direction::Up    => last.y + 1,
                    Direction::Down  => last.y - 1,
                    _ => last.y,
                },
                last.direction.clone(),
            )
        );

    }

}
