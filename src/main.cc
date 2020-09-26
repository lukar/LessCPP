#include "const_globals.h"


#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <iostream>
#include <stack>

#include "context.h"
#include "menu_context.h"




int main() {

	std::stack<Context*> contexts;

	sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Less game", sf::Style::Close);
	window.setPosition(sf::Vector2i(0, 0));
	window.setFramerateLimit(30);

	contexts.push(new MenuContext());

	sf::Event event;
	sf::Texture texture;
	if (!texture.create(window_width, window_height)) exit(1);

	while (window.isOpen()) {
		if (contexts.top()->isQuitting()) {
			delete contexts.top();
			contexts.pop();
			if (contexts.size() == 0) break;
		}

		while (window.pollEvent(event)) {
			Context* newcontext = contexts.top()->update(event, getMousePosition(window));
			if (newcontext != nullptr) contexts.push(newcontext);
		}

		window.clear();

		window.draw(sf::Sprite(contexts.top()->render(getMousePosition(window))));

		window.display();
	}

	return 0;
}
