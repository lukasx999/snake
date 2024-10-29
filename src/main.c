#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <assert.h>

#include <raylib.h>

#include "./snake.h"


#define BG_COLOR (Color) { 50, 50, 50, 0 }
#define SCREEN_WIDTH  1920
#define SCREEN_HEIGHT 1080
#define SQUARE_SIZE 20
#define TITLE "Snake"
#define FPS 60

void get_input_etc(Game *game) {

    if (IsKeyDown(KEY_I))
        snake_grow(game);

    if (IsKeyDown(KEY_U))
        snake_shrink(game);

}


void get_input_movement(Game *game) {

    if (IsKeyDown(KEY_H) || IsKeyDown(KEY_A))
        snake_move(game, LEFT);

    if (IsKeyDown(KEY_L) || IsKeyDown(KEY_D))
        snake_move(game, RIGHT);

    if (IsKeyDown(KEY_K) || IsKeyDown(KEY_W))
        snake_move(game, UP);

    if (IsKeyDown(KEY_J) || IsKeyDown(KEY_S))
        snake_move(game, DOWN);


}

void render_snake(Game *game) {

    // offset to center canvas
    int offset_x = SCREEN_WIDTH/2  - game->width  * SQUARE_SIZE/2;
    int offset_y = SCREEN_HEIGHT/2 - game->height * SQUARE_SIZE/2;

    for (    int i = 0; i < game->width;  ++i) {
        for (int j = 0; j < game->height; ++j) {

            Drawable cell = game->canvas[i][j];

            int pos_x = i * SQUARE_SIZE + offset_x;
            int pos_y = j * SQUARE_SIZE + offset_y;

            DrawRectangle(pos_x, pos_y, SQUARE_SIZE, SQUARE_SIZE, cell.color);

        }
    }

}


static void render_string(char *str,
                          int   fontsize,
                          Color color,
                          int   y_offset) {

    int stringwidth = MeasureText(str, fontsize);
    DrawText(str,
             SCREEN_WIDTH/2 - stringwidth/2,
             y_offset,
             fontsize,
             color);

}


void render_size_text(Game *game) {

    char buf[BUFSIZ] = { 0 };
    snprintf(buf, BUFSIZ, "SIZE: %d", get_snake_size(game->snake_head));
    render_string(buf, 30, RAYWHITE, 5);

}


typedef enum  {
    GAME_PAUSED,
    GAME_RUNNING,
    GAME_TITLESCREEN,
    GAME_OVER,
} GameState;







int main(void) {


    SetTargetFPS(FPS);
    SetTraceLogLevel(LOG_ERROR);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE);

    GameState state = GAME_TITLESCREEN;

    Color
    color_snake  = { 23,  118, 207, 255 },
    color_border = { 73,  77,  82,  255 },
    color_bg     = { 20,  20,  20,  255 },
    color_food   = { 20,  200, 20,  255 },
    color_wrap   = { 50,  50,  100, 255 };
    Game game    = game_new(75, 50, 0,
                            color_snake,
                            color_border,
                            color_bg,
                            color_food,
                            color_wrap,
                            5);


    float time  = 0.0f;
    float delay = 0.05f;

    while (!WindowShouldClose()) {
        BeginDrawing();
        {
            ClearBackground(BG_COLOR);

            switch (state) {

                case GAME_TITLESCREEN: {

                    render_string("Snake!", 100, BLUE, 50);
                    render_string("[Press <Space> to start]", 100,
                                  RAYWHITE, SCREEN_HEIGHT/2);

                    if (IsKeyPressed(KEY_SPACE))
                        state = GAME_RUNNING;

                } break;

                case GAME_OVER: {
                    render_string("You Lose!", 100, BLUE, 50);
                    render_string("[Press <Space> to restart]", 100,
                                  RAYWHITE, SCREEN_HEIGHT/2);

                    if (IsKeyPressed(KEY_SPACE)) {
                        // restart the game
                        game_destroy(&game);
                        game = game_new(game.width,
                                        game.height,
                                        game.snake_initial_size,
                                        game.snake_color,
                                        game.border_color,
                                        game.background_color,
                                        game.food_color,
                                        game.wrapround_color,
                                        game.snake_growth_rate);
                        state = GAME_TITLESCREEN;
                    }

                }
                    break;

                case GAME_PAUSED: {

                    render_string("Paused", 100, BLUE, 50);
                    render_string("[Press <Space> to resume]", 100,
                                  RAYWHITE, SCREEN_HEIGHT/2);

                    if (IsKeyPressed(KEY_SPACE))
                        state = GAME_RUNNING;

                } break;

                case GAME_RUNNING: {

                    if (IsKeyPressed(KEY_P))
                        state = GAME_PAUSED;

                    render_size_text(&game);
                    render_snake(&game);
                    get_input_movement(&game);
                    time += GetFrameTime();
                    if (time >= delay) {
                        get_input_etc(&game);
                        bool loss = game_update(&game);
                        if (loss)
                            state = GAME_OVER;
                        time = 0;
                    }
                } break;

            }




        }
        EndDrawing();
    }

    CloseWindow();

    game_destroy(&game);

    return EXIT_SUCCESS;
}
