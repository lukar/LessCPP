#include "block_shape.h"
#include <cassert>

Block::Block(float size, float border, WallConfig config, int rotation)
				: wall(Wall{size, size / 10, wall::rotateWallConfig(config, rotation)}) {
	this->m_size = size;
	this->m_line_width = size / 40;
	this->m_inner_size = size - 2 * border;
	this->m_border = border;

	assert(size > border);

	// Osnovna povrsina
	sf::Color brd_color(100, 100, 100);
	this->surface = sf::RectangleShape(sf::Vector2f(m_inner_size, m_inner_size));
	this->surface.setFillColor(sf::Color::White);
	this->surface.setOutlineColor(brd_color);
	this->surface.setOutlineThickness(border);
	this->surface.setOrigin(sf::Vector2f(m_size / 2, m_size / 2));

	// Vertikalna crta
	this->vertical_line = sf::RectangleShape(sf::Vector2f(m_line_width, m_inner_size));
	this->vertical_line.setFillColor(sf::Color::Red);
	this->vertical_line.setOrigin(sf::Vector2f(m_size / 2, m_size / 2));

	// Horizontalna crta
	this->horizontal_line = sf::RectangleShape(sf::Vector2f(m_inner_size, m_line_width));
	this->horizontal_line.setFillColor(sf::Color::Red);
	this->horizontal_line.setOrigin(sf::Vector2f(m_size / 2, m_size / 2));

	// Zid
	this->wall.setFillColor(sf::Color::Blue);

	this->setPosition(0.f, 0.f); // Privzeta pozicija že implementirana v funkciji
}

void Block::setPosition(float x, float y) {
	surface.setPosition(x + m_border, y + m_border);
	vertical_line.setPosition(x + m_size / 2 - m_line_width / 2, y + m_border);
	horizontal_line.setPosition(x + m_border, y + m_size / 2 - m_line_width / 2);
	wall.setPosition(x, y);
}

void Block::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	target.draw(this->surface, states);
	target.draw(this->vertical_line, states);
	target.draw(this->horizontal_line, states);
	target.draw(this->wall, states);
}

void Block::setRotation(float angle) {
	this->wall.setRotation(angle);
}

int Block::hasWall(WallSeg seg) const {
	for (auto &bseg: wall.getMConfig()) {
		if (seg == bseg) {
			return true;
		}
	}
	return false;
}

