#pragma once

#include <iostream>
#include <memory>
#include "raylib-cpp.hpp"


enum struct Direction {
    Right,
    Left,
    Up,
    Down,
};


struct SnakeNode {
public:
    Direction m_direction;
    int m_x, m_y;
    SnakeNode *m_next,
              *m_prev;
    SnakeNode(int x, int y, Direction direction);
private:
};



enum struct CellType {
    Snake,
    Background,
    Border,
    Apple,
};

struct Drawable {
public:
    raylib::Color m_color;
    CellType m_type;
    Drawable(raylib::Color color, CellType type) : m_color(color), m_type(type) {}
private:
};




class GameBuilder;



class Game {
public:
    int           m_width,
                  m_height;
    SnakeNode    *m_snake_head;
    raylib::Color m_color_bg,
                  m_color_snake;
    Direction     m_start_direction;
    std::vector<std::vector<Drawable>> m_canvas;

    Game() = default;
    Game(Game &other) = default;
    Game(int           width,
         int           height,
         raylib::Color color_bg,
         raylib::Color color_snake,
         Direction     direction = Direction::Left);

private:
};



class GameBuilder {
public:
    Game m_game;

    GameBuilder() = default;

    GameBuilder& set_width(int width) {
        m_game.m_width = width;
        return *this;
    }
    GameBuilder& set_height(int height) {
        m_game.m_height = height;
        return *this;
    }
    GameBuilder& set_color_bg(raylib::Color bg) {
        m_game.m_color_bg = bg;
        return *this;
    }
    GameBuilder& set_color_snake(raylib::Color snake) {
        m_game.m_color_snake = snake;
        return *this;
    }
    GameBuilder& set_direction(Direction direction) {
        m_game.m_start_direction = direction;
        return *this;
    }
    Game& build() {
        return m_game;
    }
private:
};
