//
// Created by aviany on 5/10/19.
//

#ifndef LESSCPP_PIECE_H
#define LESSCPP_PIECE_H

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Export.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include "const_globals.h"
#include "helpers.h"


class Piece : public sf::Drawable
{
private:

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    sf::CircleShape m_shape;
    Location m_location;

public:

    void setPosition(const sf::Vector2f &);
    void setPosition(const Location &);
    void setLocation(const Location &);
    Location getLocation() const;
    sf::Vector2f getPosition() const;
    void resetPosition();
    Piece(Location, sf::Color);
    Piece() = default;
};

#endif //LESSCPP_PIECE_H
