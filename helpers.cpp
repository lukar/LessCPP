//
// Created by aviany on 5/9/19.
//

#include "helpers.h"

sf::Vector2f getPositionFromLocation(std::pair<int, int> location) {
	int x = location.first;
	int y = location.second;
	assert(x >= 0 || x <= 5 || y >= 0 || y <= 5);
	return {
			x * (block_size / 2) + block_size / 4,
			y * (block_size / 2) + block_size / 4
	};
}

std::pair<int, int> getLocationFromPosition(const sf::Vector2f &coords) {
	assert (coords.x >= 0 || coords.x <= block_size * 3 ||
			coords.y >= 0 || coords.y <= block_size * 3
	);
	return std::pair<int, int>(static_cast<int>(coords.x) / static_cast<int>(block_size / 2),
							   static_cast<int>(coords.y) / static_cast<int>(block_size / 2));
}

sf::Vector2f getMousePosition(const sf::RenderWindow &window) {
	return static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
}
