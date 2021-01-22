#include "const_globals.h"


#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


#include <iostream>
#include <stack>

#include "context.h"
#include "main_menu_context.h"

#include "osdialog.h"


int main(int argc, char** argv)
{
	std::stack<Context*> contexts;

	sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Less game", sf::Style::Close);
	window.setPosition(sf::Vector2i(0, 0));
	window.setFramerateLimit(30);

	contexts.push(new MainMenuContext());

	sf::Event event;
	sf::Clock dtClock;
	float dt;

	while (window.isOpen()) {
		dt = dtClock.getElapsedTime().asSeconds();

		if (contexts.top()->isQuitting()) {
			for (int i = contexts.top()->getQuitLevel(); i > 0; --i) {
				delete contexts.top();
				contexts.pop();
			}
			if (contexts.size() == 0) break;
		}

		contexts.top()->update(dt, getMousePosition(window));
		while (window.pollEvent(event)) {
			Context* newcontext = contexts.top()->processEvent(event);
			if (newcontext != nullptr) contexts.push(newcontext);
		}
		// background events = true (others are ignored)
		contexts.top()->processBackgroundEvent(); 

		window.clear();

		window.draw(sf::Sprite(contexts.top()->render()));

		window.display();
	}

	return 0;
}



