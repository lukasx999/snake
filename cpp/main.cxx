#include <iostream>

#include "raylib-cpp.hpp"

#include "./game.hxx"



int main() {

    raylib::Color color_bg    = { 0,   0, 0, 0 };
    raylib::Color color_snake = { 100, 0, 0, 0 };

    Game game = GameBuilder()
        .set_width      (50)
        .set_height     (50)
        .set_color_bg   (color_bg)
        .set_color_snake(color_snake)
        .set_direction  (Direction::Left)
        .build();



    raylib::Window window(1920, 1080, "Snake!");

    window.SetTargetFPS(60);

    while (!window.ShouldClose()) {
        window.BeginDrawing();

        window.ClearBackground(RAYWHITE);

        DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

        window.EndDrawing();
    }

    return 0;
}
