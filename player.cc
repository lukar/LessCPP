//
// Created by aviany on 5/10/19.
//

#include "player.h"
#include "assert.h"


Player::Player(float size, sf::Color color, Location location) : m_size(size), m_color{color} {
	this->m_shape = sf::CircleShape(m_size);
	m_shape.setFillColor(m_color);
	m_shape.setOutlineThickness(2);
	m_shape.setOutlineColor(sf::Color::Black);
	setLocation(location);
	m_shape.setOrigin({m_size, m_size});
}

void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	target.draw(this->m_shape, states);
}

sf::Vector2f Player::getPosition() const {
	return this->m_shape.getPosition();
}

Location Player::getLocation() const {
	return this->m_location;
}

void Player::setSelected() {
	this->m_selected = true;
}

void Player::unsetSelected() {
	this->m_selected = false;
}

bool Player::isSelected() {
	return m_selected;
}

void Player::setPosition(const sf::Vector2f &position) {
	this->m_shape.setPosition(position);
}

void Player::setPosition(Location loc) {
	int x = loc[0];
	int y = loc[1];
	assert(x >= 0 && x <= 5 && y >= 0 && y <= 5);
	this->m_shape.setPosition({
																		x * (block_size / 2) + block_size / 4,
																		y * (block_size / 2) + block_size / 4
														});
}

void Player::setLocation(std::array<int, 2> location) {
	setPosition(location);
	m_location = location;
}

void Player::setLocation(const sf::Vector2f &coords) {
	assert (coords.x >= 0 || coords.x <= block_size * 3 ||
					coords.y >= 0 || coords.y <= block_size * 3
	);
	m_location = {static_cast<int>(coords.x / (block_size / 2)),
								static_cast<int>(coords.y / (block_size / 2))};
}

std::string Player::toString() const {
	sf::Vector2f position = this->m_shape.getPosition();
	return std::string(std::to_string(position.x) + ", " + std::to_string(position.y));
}
