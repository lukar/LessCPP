#include <cassert>
#include "wall.hh"


Wall::Wall(float size, float width, int config) {
	m_size = size;
	m_width = width;
	assert(size > width);
	m_wall = wall::wall_configs[config];

	for (auto &w : m_wall) {
		sf::RectangleShape a_wall{(w[0] == 1
								   ? sf::Vector2f(m_width, m_size / 2 + m_width / 2)
								   : sf::Vector2f(m_size / 2 + m_width / 2, m_width))};
		a_wall.setOrigin(-getWallSegOrigin(w));
		m_wallShape.push_back(a_wall);
	}
}

void Wall::setFillColor(const sf::Color &color) {
	for (auto &w : m_wallShape) {
		w.setFillColor(color);
	}
}

void Wall::setPosition(float x, float y) {
	for (auto &w : m_wallShape) {
		w.setPosition(sf::Vector2f(x, y));
	}
}

void Wall::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	for (const auto &w : m_wallShape)
		target.draw(w, states);
}

void Wall::setRotation(float angle) {
	for (auto &w : m_wallShape) {
		w.setRotation(angle);
	}
}

sf::Vector2f Wall::getWallSegOrigin(std::vector<int> &wallSeg) {
	float x_b = -m_width / 2, y_b = x_b;
	if (wallSeg[0] == 1) // vertikalno
	{
		x_b += wallSeg[1] * (m_size / 2 - m_width / 2);
		y_b -= (wallSeg[2] == -1 ? m_size / 2 - m_width / 2 : 0);
	} else // horizontalno
	{
		y_b += wallSeg[1] * (m_size / 2 - m_width / 2);
		x_b -= (wallSeg[2] == -1 ? m_size / 2 - m_width / 2 : 0);
	}
	return {x_b, y_b};
}
