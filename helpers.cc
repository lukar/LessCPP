//
// Created by aviany on 5/9/19.
//

#include "helpers.h"
#include <cassert>


sf::Vector2f getMousePosition(const sf::RenderWindow &window) {
	return static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
}

Location getMouseLocation(const sf::RenderWindow &window) {
	auto mouse = sf::Mouse::getPosition(window);
	assert (mouse.x >= 0 || mouse.x <= block_size * 3 ||
			mouse.y >= 0 || mouse.y <= block_size * 3
	);
	return {static_cast<int>(mouse.x / (block_size / 2)),
			static_cast<int>(mouse.y / (block_size / 2))};
}
