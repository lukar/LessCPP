#include "wall.hh"

namespace sf {
WallShape::WallShape(float width, float length, int rotation, int config)
    : m_rotation(rotation), m_width(width), m_length(length),
      m_wall(wall::wall_configs[config]) {
  for (auto &w : m_wall) {
    float dx = (w[0] == 1 ? width : length);
    float dy = (w[0] == 1 ? length : width);
    RectangleShape a_wall{Vector2f(dx, dy)};
    a_wall.setFillColor(Color::Blue);
    m_wallShape.push_back(a_wall);
  }

  this->setPosition(0.f, 0.f);
}

void WallShape::setPosition(float x, float y) {
  int len = m_wall.size();
  for (int i = 0; i < len; ++i) {
    float y_b = m_length - m_width;
    float x_b = m_length - m_width;
    if (m_wall[i][0] == 1) // vertikalno
    {
      x_b += m_wall[i][1] * (m_length - m_width);
      y_b -= (m_wall[i][2] == -1 ? m_length - m_width : 0);
    } else // horizontalno
    {
      y_b += m_wall[i][1] * (m_length - m_width);
      x_b -= (m_wall[i][2] == -1 ? m_length - m_width : 0);
    }
    x_b += x;
    y_b += y;
    this->m_wallShape[i].setPosition(x_b, y_b);
  }
}

void WallShape::draw(RenderTarget &target, RenderStates states) const {
  for (auto &sf : this->m_wallShape)
    target.draw(sf, states);
}

}; // namespace sf
