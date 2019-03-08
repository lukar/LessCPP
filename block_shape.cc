#include "block_shape.hh"

namespace sf
{
BlockShape::BlockShape(const float size, const float border)
    : m_size(size), m_line_width(size / 20), m_inner_size(size - 2 * border), m_border(border)
{
    sf::Color brd_color(100, 100, 100);
    this->surface = RectangleShape(Vector2f(m_inner_size, m_inner_size));
    this->surface.setFillColor(Color::White);
    this->surface.setOutlineColor(brd_color);
    this->surface.setOutlineThickness(border);

    this->vertical_line = RectangleShape(Vector2f(m_line_width, m_inner_size));
    this->vertical_line.setFillColor(Color::Red);

    this->horizontal_line = RectangleShape(Vector2f(m_inner_size, m_line_width));
    this->horizontal_line.setFillColor(Color::Red);

    this->setPosition(0.f, 0.f); // Privzeta pozicija že implementirana v funkciji
}

void BlockShape::setPosition(float x, float y)
{
    this->surface.setPosition(x + m_border, y + m_border);
    this->vertical_line.setPosition(x + m_size / 2 - m_line_width / 2, y + m_border);
    this->horizontal_line.setPosition(x + m_border, y + m_size / 2 - m_line_width / 2);
}

void BlockShape::draw(RenderTarget& target, RenderStates states) const
{
    target.draw(this->surface, states);
    target.draw(this->vertical_line, states);
    target.draw(this->horizontal_line, states);
}
} // namespace sf
