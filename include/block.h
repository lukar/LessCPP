#ifndef BLOCK_H
#define BLOCK_H


#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Export.hpp>
#include <SFML/Graphics/Shape.hpp>
#include "wall.h"
#include "const_globals.h"
#include <vector>


class Block : public sf::Drawable
{
private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    sf::Vector2f getWallSegOrigin(const WallSeg&) const;
    std::vector<sf::RectangleShape> generateWall(const WallConfig&) const;


    sf::RectangleShape m_surface;
    sf::RectangleShape m_vertical_line;
    sf::RectangleShape m_horizontal_line;

    std::vector<sf::RectangleShape> m_wall;

public:
    Block(WallConfig config);
    Block() = default;
    void setPosition(float, float);
};


#endif // BLOCK_H
