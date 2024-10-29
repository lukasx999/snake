#include <memory>

#include "./game.hxx"





SnakeNode::SnakeNode(int x, int y, Direction direction)
    : m_direction(direction)
    , m_x        (x)
    , m_y        (y)
    , m_next     (nullptr)
    , m_prev     (nullptr)
{}


Game::Game(int           width,
           int           height,
           raylib::Color color_bg,
           raylib::Color color_snake,
           Direction     direction)
    : m_width          (width)
    , m_height         (height)
    , m_snake_head     (new SnakeNode(m_width/2, m_height/2, direction))
    , m_color_bg       (color_bg)
    , m_color_snake    (color_snake)
    , m_start_direction(direction)
{

    Drawable bg(m_color_bg, CellType::Background);

    for (auto x = 0; x < m_width; ++x) {
        m_canvas.push_back(std::vector<Drawable>());
        for (auto y = 0; y < m_height; ++y)
            m_canvas[x].push_back(bg);
    }

}





