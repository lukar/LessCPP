//
// Created by aviany on 5/10/19.
//

#include "player.h"
#include "helpers.h"

Player::Player(float size, sf::Color color, std::pair<int, int> location) : m_size(size), m_color{color} {
	this->m_shape = sf::CircleShape(m_size);
	m_shape.setFillColor(m_color);
	m_shape.setOutlineThickness(2);
	m_shape.setOutlineColor(sf::Color::Black);
	setLocation(location);
	m_shape.setOrigin({m_size, m_size});
}

void Player::setLocation(std::pair<int, int> location) {
	m_location = location;
	m_shape.setPosition(getPositionFromLocation(m_location));
}


void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	target.draw(this->m_shape, states);
}

sf::Vector2f Player::getPosition() {
	return this->m_shape.getPosition();
}

std::pair<int, int> Player::getLocation() {
	return this->m_location;
}

void Player::setSelected() {
	this->m_selected = true;
}

void Player::unsetSelected() {
	this->m_selected = false;
}

void Player::setPosition(const sf::Vector2f &position) {
	this->m_shape.setPosition(position);
}


bool Player::isSelected() {
	return m_selected;
}

std::string Player::toString() const {
	sf::Vector2f position = this->m_shape.getPosition();
	return std::string(std::to_string(position.x) + ", " + std::to_string(position.y));
}
