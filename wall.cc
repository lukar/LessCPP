#include <cassert>
#include "wall.hh"


Wall::Wall(float size, float width, int rotation, int config) {
	this->m_rotation = rotation;
	this->m_size = size;
	this->m_width = width;
	assert(size > width);
	this->m_wall = wall::wall_configs[config];

	for (auto &w : m_wall) {
		sf::RectangleShape a_wall{(w[0] == 1
								   ? sf::Vector2f(width, size / 2 + width / 2)
								   : sf::Vector2f(size / 2 + width / 2, width))};
		m_wallShape.push_back(a_wall);
	}

	this->setPosition(0.f, 0.f);
}

void Wall::setFillColor(const sf::Color &color) {
	for (auto &w : this->m_wallShape) {
		w.setFillColor(color);
	}
}

void Wall::setPosition(float x, float y) {
	int len = m_wall.size();
	for (int i = 0; i < len; ++i) {
		float x_b = m_size / 2 - m_width / 2, y_b = x_b;
		if (m_wall[i][0] == 1) // vertikalno
		{
			x_b += m_wall[i][1] * (m_size / 2 - m_width / 2);
			y_b -= (m_wall[i][2] == -1 ? m_size / 2 - m_width / 2 : 0);
		} else // horizontalno
		{
			y_b += m_wall[i][1] * (m_size / 2 - m_width / 2);
			x_b -= (m_wall[i][2] == -1 ? m_size / 2 - m_width / 2 : 0);
		}
		x_b += x;
		y_b += y;
		this->m_wallShape[i].setPosition(x_b, y_b);
	}
}

void Wall::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	for (auto &w : this->m_wallShape)
		target.draw(w, states);
}
