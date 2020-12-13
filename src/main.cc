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
	sf::Texture texture;
	if (!texture.create(window_width, window_height)) exit(1);

	while (window.isOpen()) {

		if (contexts.top()->isQuitting()) {
			for (int i = contexts.top()->getQuitLevel(); i > 0; --i) {
				delete contexts.top();
				contexts.pop();
			}
			if (contexts.size() == 0) break;
		}

		const auto mouse_pos = getMousePosition(window);
		while (window.pollEvent(event)) {
			Context* newcontext = contexts.top()->update(event, mouse_pos);
			if (newcontext != nullptr) contexts.push(newcontext);
		}

		window.clear();

		window.draw(sf::Sprite(contexts.top()->render(mouse_pos)));

		window.display();
	}

	return 0;
}
