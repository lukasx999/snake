#pragma once
#ifndef _SNAKE_H
#define _SNAKE_H

#include <raylib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>



typedef enum {
    UP,
    DOWN,
    RIGHT,
    LEFT,
} Direction;

typedef struct SnakeNode {
    Direction direction;
    int x, y;
    struct SnakeNode *next;
    struct SnakeNode *previous;
} SnakeNode;

enum CellType {
    CELL_SNAKE,
    CELL_BORDER,
    CELL_APPLE,
    CELL_BACKGROUND,
    CELL_WRAPAROUND,
};

typedef struct {
    enum CellType type; // will be checked in snake collision
    Color color;
} Drawable; // represents one colored pixel on the screen

typedef struct {
    int width, height;
    Drawable **canvas;
    SnakeNode *snake_head;
    Color snake_color,
          border_color,
          background_color,
          food_color,
          wrapround_color;
    int snake_initial_size,
        apple_x,
        apple_y,
        snake_growth_rate;
} Game;

extern Game game_new      (int   width,        int   height,
                           int   snake_size,   Color snake_color,
                           Color border_color, Color background_color,
                           Color food_color,   Color wrapround_color,
                           int   snake_growth_rate);
extern bool game_update   (Game *game); // returns true if player loses
extern void game_destroy  (Game *game);
extern void snake_grow    (Game *game);
extern void snake_shrink  (Game *game);
extern void snake_move    (Game *game, Direction direction);
extern int  get_snake_size(SnakeNode *head);



/*
Explaination:
The snake is represented by A doubly linked list of multiple nodes

Each Node has a position and a direction
Updating the snake:
- Move each node in its corresponding direction
- Each node inherits the direction of the previous node
- The head node keeps its direction, unless changed by `snake_move()`

Updating the canvas:
- Clear the canvas (otherwise snake nodes will stick around)
- Insert the borders
- Traverse the snake nodes and add them to the canvas

Now the renderer just needs to render a 2D-Array
*/


#endif // _SNAKE_H
