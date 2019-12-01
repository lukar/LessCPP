//
// Created by aviany on 5/9/19.
//

#include "helpers.h"
#include <cassert>

extern sf::RenderWindow window;

sf::Vector2f getMousePosition(sf::RenderWindow  const& window) {
	return static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
}

std::optional<Location> getMouseLocation(sf::RenderWindow  const& window) {
	auto mouse = sf::Mouse::getPosition(window);
	if (mouse.x < 0 || mouse.x >= block_size * 3 ||
			mouse.y < 0 || mouse.y >= block_size * 3)
		return {};
    return Location{static_cast<unsigned int>(mouse.x / (block_size / 2)),
                                    static_cast<unsigned int>(mouse.y / (block_size / 2))};
}
