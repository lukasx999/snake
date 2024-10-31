#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#include "./snake.h"


#define ERROR(message) { \
    perror(message);     \
    exit(1);             \
}


static SnakeNode* _snake_create(int x, int y, Direction direction) {
    SnakeNode *new = malloc(sizeof(SnakeNode));
    if (new == NULL)
        ERROR("Failed to allocate memory");
    new->x = x;
    new->y = y;
    new->direction = direction;
    new->next = NULL;
    new->previous = NULL;
    return new;
}


// removes from the tail of the snake
void snake_shrink(Game *game) {

    SnakeNode *current = game->snake_head;

    if (current->next == NULL)
        return;

    while (current->next != NULL)
        current = current->next;

    current->previous->next = NULL;
    free(current);

}



// appends to the tail of the snake
void snake_grow(Game *game) {

    // get last node
    SnakeNode *current = game->snake_head;
    while (current->next != NULL)
        current = current->next;

    // new tail node inherits direction from parent
    SnakeNode *new = _snake_create(current->x, current->y, current->direction);

    switch (current->direction) {
        case RIGHT: {
            new->x = current->x - 1;
        } break;

        case LEFT: {
            new->x = current->x + 1;
        } break;

        case UP: {
            new->y = current->y + 1;
        } break;

        case DOWN: {
            new->y = current->y - 1;
        } break;
    }

    new->previous = current;
    current->next = new;

}




static void _clear_canvas(Game *game) {

    for (    int i = 0; i < game->width;  ++i)
        for (int j = 0; j < game->height; ++j)
            game->canvas[i][j] = (Drawable) {
                .color = game->background_color,
                .type = CELL_BACKGROUND,
            };

}


static void _spawn_apple(Game *game) {

    int min = 5;
    game->apple_x = rand() % ((game->width-5) + 1 - min) + min;
    game->apple_y = rand() % ((game->height-5) + 1 - min) + min;

}



Game game_new(int   width,        int   height,
              int   snake_size,   Color snake_color,
              Color border_color, Color background_color,
              Color food_color,   Color wrapround_color,
              int   snake_growth_rate) {


    // spawn the snake in the middle of the canvas
    // default direction is `LEFT`
    SnakeNode *head = _snake_create(width/2, height/2, LEFT);

    // allocate 2d array
    Drawable **canvas = malloc(width * sizeof(Drawable*));
    if (canvas == NULL) ERROR("Failed to allocate memory");

    for (int i = 0; i < width; ++i) {
        canvas[i] = malloc(height * sizeof(Drawable));
        if (canvas[i] == NULL) ERROR("Failed to allocate memory");
    }

    Game game =  {
        .width              = width,
        .height             = height,
        .canvas             = canvas,
        .snake_head         = head,
        .snake_color        = snake_color,
        .snake_initial_size = snake_size,
        .border_color       = border_color,
        .background_color   = background_color,
        .food_color         = food_color,
        .snake_growth_rate  = snake_growth_rate,
        .wrapround_color    = wrapround_color,
    };

    // initialize canvas
    _clear_canvas(&game);

    // grow snake to initial size
    for (int _ = 0; _ < game.snake_initial_size; ++_)
        snake_grow(&game);


    // spawn initial apple
    srand(time(NULL));
    _spawn_apple(&game);


    return game;

}

void snake_move(Game *game, Direction direction) {
    Direction *d = &game->snake_head->direction;

    // dont let the player move in the opposite direction
    if ((*d == RIGHT && direction == LEFT ) ||
        (*d == LEFT  && direction == RIGHT) ||
        (*d == UP    && direction == DOWN ) ||
        (*d == DOWN  && direction == UP   )) return;

    *d = direction;
}


static void _snake_update(Game *game) {

    // get last node
    SnakeNode *current = game->snake_head;
    while (current->next != NULL)
        current = current->next;

    // update positions
    while (current != NULL) {

        switch (current->direction) {

            case RIGHT: {
                current->x++;
            } break;

            case LEFT: {
                current->x--;
            } break;

            case UP: {
                current->y--;
            } break;

            case DOWN: {
                current->y++;
            } break;

        }

        // dont set direction of head node, as it will
        // A: cause a segfault
        // B: only be manipulated by the user by calling `snake_move()`
        if (current->previous != NULL)
            current->direction = current->previous->direction;
        current = current->previous;

    }

}


// populates `neighbours` with the 4 adjacent elements of `x` and `y` in `canvas`
static void _fill_adjacent_elements(Drawable **neighbours, int x, int y, Drawable **canvas) {

    size_t i = 0;
    neighbours[i++] = &canvas[x-1][y]; // left
    neighbours[i++] = &canvas[x+1][y]; // right
    neighbours[i++] = &canvas[x]  [y+1]; // down
    neighbours[i++] = &canvas[x]  [y-1]; // up

}


static void _snake_wrapround(Game *game) {

    SnakeNode *current = game->snake_head;

    while (current != NULL) {

        Drawable *next_block = NULL;

        switch (current->direction) {

            case LEFT: {
                next_block = &game->canvas[current->x-1][current->y];
                if (next_block->type == CELL_WRAPAROUND)
                    current->x = game->width - 1;

            } break;

            case RIGHT: {
                next_block = &game->canvas[current->x+1][current->y];
                if (next_block->type == CELL_WRAPAROUND)
                    current->x = 0;

            } break;

            case UP: {
                next_block = &game->canvas[current->x][current->y-1];
                if (next_block->type == CELL_WRAPAROUND)
                    current->y = game->height - 1;

            } break;

            case DOWN: {
                next_block = &game->canvas[current->x][current->y+1];
                if (next_block->type == CELL_WRAPAROUND)
                    current->y = 0;

            } break;
        }

        current = current->next;

    }

}




// returns true on collision else false
static bool _snake_check_collision(Game *game) {

    SnakeNode *head = game->snake_head;

    Drawable *neighbours[4] = { 0 };
    _fill_adjacent_elements(neighbours, head->x, head->y, game->canvas);

    for (size_t i = 0; i < 4; ++i) {
        switch (neighbours[i]->type) {
            case CELL_SNAKE: {
                // the node immediately connected to the head of the snake
                Drawable *next = &game->canvas[head->next->x][head->next->y];

                // this node is always connected, hence we should ignore it when checking for self collisions
                if (neighbours[i] != next) {
                    return true;
                }

            } break;
            case CELL_APPLE: { // TODO: only check snake head, not all 4 directions
                for (int _; _ < game->snake_growth_rate; ++_)
                    snake_grow(game);
                _spawn_apple(game);
            } break;
            case CELL_BORDER: {
                return true;
            } break;
            default: {} break;

        }

    }

    _snake_wrapround(game);

    return false;

}




int get_snake_size(SnakeNode *head) {
    SnakeNode *current = head;
    int count = 0;

    while (current != NULL) {
        ++count;
        current = current->next;
    }

    return count;

}


bool game_update(Game *game) {

    // clear out canvas
    _clear_canvas(game);

    // insert snake into canvas
    SnakeNode *current = game->snake_head;
    while (current != NULL) {
        Drawable cell_snake = {
            .color = game->snake_color,
            .type  = CELL_SNAKE,
        };
        game->canvas[current->x][current->y] = cell_snake;
        current = current->next;
    }

    Drawable cell_border = {
        .color = game->wrapround_color,
        .type  = CELL_WRAPAROUND,
    };

    // top border
    for (int i = 0; i < game->width; ++i)
        game->canvas[i][0] = cell_border;

    // bottom border
    for (int i = 0; i < game->width; ++i)
        game->canvas[i][game->height-1] = cell_border;

    // left border
    for (int i = 0; i < game->height; ++i)
        game->canvas[0][i] = cell_border;

    // right border
    for (int i = 0; i < game->height; ++i)
        game->canvas[game->width-1][i] = cell_border;



    Drawable apple = { .type = CELL_APPLE, .color = game->food_color };
    game->canvas[game->apple_x][game->apple_y] = apple;


    bool collision = _snake_check_collision(game);


    _snake_update(game);
    return collision;

}



void game_destroy(Game *game) {

    for (int i = 0; i < game->width; ++i)
        free(game->canvas[i]);
    free(game->canvas);

    SnakeNode *current = game->snake_head;
    while (current != NULL) {
        SnakeNode *next = current->next;
        free(current);
        current = next;
    }

}
