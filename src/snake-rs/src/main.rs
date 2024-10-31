use raylib::prelude::*;

mod game;
use game::{Game, Drawable};


const SCREEN_WIDTH:  i32 = 1920;
const SCREEN_HEIGHT: i32 = 1080;
const RECT_SIZE:     i32 = 20;
const DELAY:         f32 = 0.05;





fn main() {


    let (mut rl, thread) = raylib::init()
        .size(SCREEN_WIDTH, SCREEN_HEIGHT)
        .title("Hello, World")
        .log_level(TraceLogLevel::LOG_ERROR)
        .build();

    rl.set_target_fps(60);

    let mut game = Game::new(50u32, 50u32, 5usize);


    let mut time: f32 = 0.0;



    while !rl.window_should_close() {
        let mut d = rl.begin_drawing(&thread);

        d.clear_background(Color::WHITE);


        if d.is_key_pressed(KeyboardKey::KEY_H) {
            game.snake_move(game::Direction::Left);
        }

        if d.is_key_pressed(KeyboardKey::KEY_L) {
            game.snake_move(game::Direction::Right);
        }

        if d.is_key_pressed(KeyboardKey::KEY_J) {
            game.snake_move(game::Direction::Down);
        }

        if d.is_key_pressed(KeyboardKey::KEY_K) {
            game.snake_move(game::Direction::Up);
        }

        time += d.get_frame_time();
        if time >= DELAY {
            game.update();

            if d.is_key_down(KeyboardKey::KEY_SPACE) {
                game.snake_grow();
            }

            time = 0.0;
        }

        for x in 0..game.width {
            for y in 0..game.height {

                let cell: &Drawable = &game.canvas[x as usize][y as usize];

                let adj_x = x as i32 * RECT_SIZE + SCREEN_WIDTH/2 -
                    game.width as i32 / 2 * RECT_SIZE;

                let adj_y = y as i32 * RECT_SIZE + SCREEN_HEIGHT/2 -
                    game.height as i32 / 2 * RECT_SIZE;

                d.draw_rectangle(adj_x, adj_y,
                    RECT_SIZE, RECT_SIZE, cell.color);

            }
        }



    }
}
