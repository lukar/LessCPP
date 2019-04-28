#ifndef WALL_H
#define WALL_H

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Export.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <vector>

namespace wall {
	typedef std::vector<std::vector<int>> WallConfig;

// static variabla garantira, da bo obstajala samo ena kopija na .cc fajl
// (Brez tega ne dela!)
	static const WallConfig bFullSide = {{1, -1, -1},
										 {1, -1, 1}};
	static const WallConfig bHalfSide = {{1, -1, 1}};
	static const WallConfig bZigZag = {{1,  -1, -1},
									   {-1, 0,  -1},
									   {1,  0,  1}};
	static const WallConfig bT_Block = {{1,  -1, -1},
										{1,  -1, 1},
										{-1, 0,  -1}};
	static const WallConfig bTopLeft = {{1,  -1, -1},
										{-1, 0,  -1}};
	static const WallConfig bTopRight = {{1,  0, -1},
										 {-1, 0, 1}};
	static const WallConfig bBottomRight = {{1,  0, 1},
											{-1, 1, 1}};

	static std::vector<WallConfig> wall_configs{
			bFullSide, bHalfSide, bZigZag, bT_Block, bTopLeft, bTopRight, bBottomRight};
} // namespace wall


class Wall : public sf::Drawable {
private:
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

	int m_rotation;
	float m_size;
	float m_width;
	wall::WallConfig m_wall;
	std::vector<sf::RectangleShape> m_wallShape;
public:
	explicit Wall(float size = 0, float width = 0, int rotation = 0, int config = 0);

	void setPosition(float x, float y);

	void setFillColor(const sf::Color &color);
};

#endif /* WALL_H */
