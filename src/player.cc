//
// Created by aviany on 5/10/19.
//

#include "player.h"

Player::Player(Location location, sf::Color color) : m_location(location) {
    m_shape = sf::CircleShape(player_size);
    m_shape.setFillColor(color);
    m_shape.setOutlineThickness(border_width);
    m_shape.setOutlineColor(sf::Color::Black);
    m_shape.setOrigin({player_size, player_size});
    setPosition(m_location);
}

void Player::setPosition(const sf::Vector2f &position) {
    m_shape.setPosition(position);
}

void Player::setPosition(const Location &location) {
    m_shape.setPosition(positionFromLocation(location));
}

void Player::setLocation(const Location & location) {
    m_location = location;
}

void Player::resetPosition() {
    setPosition(m_location);
}

void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(m_shape, states);
}

Location Player::getLocation() const {
    return m_location;
}

sf::Vector2f Player::getPosition() const {
    return m_shape.getPosition();
}
