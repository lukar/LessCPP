#include <cassert>
#include "wall.h"


WallConfig wall::rotateWallConfig(WallConfig const config, int const rotation) {
	WallConfig tmp;
	int orientation;
	int displacement;
	int distance;

	for (int i = 0; i < 3; ++i) {
		if (config[i][0] == -2) {
			tmp[i] = {-2, 0, 0};
			continue;
		}
		orientation = config[i][0];
		displacement = config[i][1];
		distance = config[i][2];

		for (int j = 0; j < rotation; ++j) {
			displacement = orientation * displacement;
			distance = -orientation * distance;
			orientation = -orientation;
		}
		tmp[i] = {orientation, displacement, distance};
	}
	return tmp;
}


Wall::Wall(float size, float width, WallConfig config) : m_size(size), m_width(width), m_config(config) {

	for (auto &w : m_config) {
		if (w[0] == -2) continue; // -2 means there is no wall
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

sf::Vector2f Wall::getWallSegOrigin(std::array<int, 3> &wallSeg) {
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

const WallConfig &Wall::getMConfig() const {
	return m_config;
}
