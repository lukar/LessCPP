//
// Created by aviany on 5/10/19.
//

#ifndef LESSCPP_PLAYER_H
#define LESSCPP_PLAYER_H

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Export.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include "const_globals.h"


class Player : public sf::Drawable {
private:
		void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

		float m_size;

		sf::CircleShape m_shape;
		Location m_location;
		sf::Color m_color;
		bool m_selected = false;

public:
		Player(float size = 0, sf::Color color = sf::Color::White, std::pair<int, int> location = {0, 0});

		void setLocation(Location);

		void setLocation(const sf::Vector2f &);

		void setPosition(const sf::Vector2f &);

		void setPosition(Location);

		void setSelected();

		void unsetSelected();

		sf::Vector2f getPosition() const;

		Location getLocation() const;

		bool isSelected();

		std::string toString() const;
};


extern std::vector<Player> whitePlayers;

extern std::vector<Player> blackPlayers;

#endif //LESSCPP_PLAYER_H
