#ifndef WALL_H
#define WALL_H

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Export.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <array>
#include "const_globals.h"

namespace wall {

		constexpr WallConfig bFullSide = {
						{
										{1, -1, -1},
										{1, -1, 1},
										{-2, 0, 0}
						}
		};

		constexpr WallConfig bHalfSide = {
						{
										{1, -1, 1},
										{-2, 0, 0},
										{-2, 0, 0}

						}

		};
		constexpr WallConfig bZigZag = {
						{
										{1, -1, -1},
										{-1, 0, -1},
										{1, 0, 1}
						}
		};
		constexpr WallConfig bT_Block = {
						{
										{1, -1, -1},
										{1, -1, 1},
										{-1, 0, -1}
						}
		};
		constexpr WallConfig bTopLeft = {
						{
										{1, -1, -1},
										{-1, 0, -1},
										{-2, 0, 0}

						}
		};
		constexpr WallConfig bTopRight = {
						{
										{1, 0, -1},
										{-1, 0, 1},
										{-2, 0, 0}
						}
		};
		constexpr WallConfig bBottomRight = {
						{
										{1, 0, 1},
										{-1, 1, 1},
										{-2, 0, 0}
						}
		};


		constexpr std::array<WallConfig, 7> wall_configs{
						bFullSide, bHalfSide, bZigZag, bT_Block, bTopLeft, bTopRight, bBottomRight};


		WallConfig rotateWallConfig(WallConfig wall, int rotation);

} // namespace wall

#endif /* WALL_H */

class Wall : public sf::Drawable {
private:
		void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

		float m_size;
		float m_width;
		std::vector<sf::RectangleShape> m_wallShape;
		WallConfig m_config;
public:
		const WallConfig &getMConfig() const;

		explicit Wall(float size = 0, float width = 0, WallConfig config = wall::bFullSide);

		void setPosition(float x, float y);

		void setRotation(float angle);

		void setFillColor(const sf::Color &color);

		sf::Vector2f getWallSegOrigin(std::array<int, 3> &);


};

