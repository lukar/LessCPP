#ifndef BLOCK_SHAPE_H
#define BLOCK_SHAPE_H

#include "wall.hh"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Export.hpp>
#include <SFML/Graphics/Shape.hpp>


class Block : public sf::Drawable {
private:
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

	sf::RectangleShape surface;
	sf::RectangleShape vertical_line;
	sf::RectangleShape horizontal_line;
	Wall wall;
	float m_size; //whole size
	float m_line_width; //wall width
	float m_inner_size; //size without borders
	float m_border; //border width

public:
	explicit Block(float size, float border);

	void setPosition(float x, float y);
};

#endif /* BLOCK_SHAPE_H */
