//
// Created by aviany on 5/10/19.
//

#include "piece.h"

Piece::Piece(Location location, sf::Color color) : m_location(location) {
    m_shape = sf::CircleShape(piece_size);
    m_shape.setFillColor(color);
    m_shape.setOutlineThickness(border_width);
    m_shape.setOutlineColor(sf::Color::Black);
    m_shape.setOrigin({piece_size, piece_size});
    setPosition(m_location);
}

void Piece::setPosition(const sf::Vector2f &position) {
    m_shape.setPosition(position);
}

void Piece::setPosition(const Location &location) {
    m_shape.setPosition(positionFromLocation(location));
}

void Piece::setLocation(const Location & location) {
    m_location = location;
}

void Piece::resetPosition() {
    setPosition(m_location);
}

void Piece::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(m_shape, states);
}

Location Piece::getLocation() const {
    return m_location;
}

sf::Vector2f Piece::getPosition() const {
    return m_shape.getPosition();
}
