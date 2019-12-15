#include "block.h"

Block::Block(WallConfig config) {
    // Osnovna povrsina
    const float inner_size = block_size - 2*border_width;

    m_surface = sf::RectangleShape(sf::Vector2f(inner_size, inner_size));
    m_surface.setFillColor(sf::Color::White);
    m_surface.setOutlineColor(sf::Color(100, 100, 100)); // gray color
    m_surface.setOutlineThickness(border_width);
    m_surface.setOrigin(sf::Vector2f(block_size / 2, block_size / 2));

    // Vertikalna crta
    m_vertical_line = sf::RectangleShape(sf::Vector2f(line_width, inner_size));
    m_vertical_line.setFillColor(sf::Color::Red);
    m_vertical_line.setOrigin(sf::Vector2f(block_size / 2, block_size / 2));

    // Horizontalna crta
    m_horizontal_line = sf::RectangleShape(sf::Vector2f(inner_size, line_width));
    m_horizontal_line.setFillColor(sf::Color::Red);
    m_horizontal_line.setOrigin(sf::Vector2f(block_size / 2, block_size / 2));

    // Zid
    m_wall = generateWall(config);

    setPosition(0.f, 0.f); // Privzeta pozicija že implementirana v funkciji
}



void Block::setPosition(float x, float y) {
        m_surface.setPosition(x + border_width, y + border_width);
        m_vertical_line.setPosition(x + block_size / 2 - line_width / 2, y + border_width);
        m_horizontal_line.setPosition(x + border_width, y + block_size / 2 - line_width / 2);
        for (auto &w : m_wall) {
            w.setPosition(sf::Vector2f(x, y));
        }
}

void Block::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(m_surface, states);
    target.draw(m_vertical_line, states);
    target.draw(m_horizontal_line, states);
    for (auto &w : m_wall) {
        target.draw(w, states);
    }
}

sf::Vector2f Block::getWallSegOrigin(const WallSeg &wallSeg) const {
    float x_b, y_b;
    x_b = y_b = wall_width / 2; // initial position
    float offset = (block_size - wall_width) / 2; // offset
    x_b -= wallSeg[1] * offset;
    y_b += wallSeg[2] == -1 ? offset : 0;
    if( wallSeg[0] != 1) std::swap(x_b, y_b);
    return {x_b, y_b};
}


std::vector<sf::RectangleShape> Block::generateWall(const WallConfig &config) const {
    std::vector<sf::RectangleShape> tmp;
    for (auto &w : config) {
        if (w[0] == -2) continue; // -2 means there is no wall
        sf::RectangleShape a_wall{(w[0] == 1
                                   ? sf::Vector2f(wall_width, block_size / 2 + wall_width / 2)
                                   : sf::Vector2f(block_size / 2 + wall_width / 2, wall_width))};
        a_wall.setOrigin(getWallSegOrigin(w));
        a_wall.setFillColor(sf::Color::Blue);
        tmp.push_back(a_wall);
    }
    return tmp;
}
