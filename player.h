//
// Created by aviany on 5/10/19.
//

#ifndef LESSCPP_PLAYER_H
#define LESSCPP_PLAYER_H

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Export.hpp>
#include <SFML/Graphics/CircleShape.hpp>

class Player : public sf::Drawable {
private:
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

	const float m_size;

	sf::CircleShape m_shape;
	std::pair<int, int> m_location;
	sf::Color m_color;
	bool m_selected = false;

public:
	Player(float size = 0, sf::Color color = sf::Color::White, std::pair<int, int> location = {0, 0});

	void setLocation(std::pair<int, int>);

	void setPosition(const sf::Vector2f &);

	void setSelected();

	void unsetSelected();

	sf::Vector2f getPosition();

	std::pair<int, int> getLocation();

	bool isSelected();

	std::string toString() const;

	friend std::string &operator<<(std::ostream &os, const Player &player);

};


#endif //LESSCPP_PLAYER_H
