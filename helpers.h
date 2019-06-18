//
// Created by aviany on 5/5/19.
//

#ifndef LESSCPP_HELPERS_H
#define LESSCPP_HELPERS_H

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <optional>
#include "const_globals.h"


template<typename T, typename R>
float euclideanDistance(const sf::Vector2<T> &pointA, const sf::Vector2<R> &pointB) {
	return sqrt(
					pow(pointA.x - pointB.x, 2) +
					pow(pointA.y - pointB.y, 2)
	);
}

sf::Vector2f getMousePosition(const sf::RenderWindow &);

std::optional<Location> getMouseLocation(const sf::RenderWindow &);

#endif //LESSCPP_HELPERS_H
