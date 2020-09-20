#include "const_globals.h"


#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <iostream>
#include <stack>

#include "context.h"
#include "game_context.h"




int main() {

	std::stack<Context*> contexts;

	sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Less game", sf::Style::Close);
	window.setPosition(sf::Vector2i(0, 0));
	window.setFramerateLimit(30);

	contexts.push(new GameContext());

	sf::Event event;
	sf::Texture texture;
	if (!texture.create(window_width, window_height)) exit(1);

	while (window.isOpen()) {

		while (window.pollEvent(event)) {
			window.clear();
			Result result = contexts.top()->process(event, getMousePosition(window));
			sf::Texture texture = std::get<sf::Texture>(result);
			window.draw(sf::Sprite(texture));

			window.display();
		}



	}

	return 0;
}
