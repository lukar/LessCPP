#include "menu_context.h"
#include "ai.h"
#include <tuple>

MenuContext::MenuContext()
{
	rentex.create(window_width, window_height);

	// generate side text
	font = getFont("resources/Roboto_Medium.ttf");
	text = initializePlayerText(font, window_width/2, window_height/2, sf::Color::Green);
}

Context* MenuContext::update(const sf::Event & event, const sf::Vector2f & mouse_pos)
{
	if (event.type == sf::Event::MouseButtonPressed) {
		return new GameContext();
	}
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Q) {
			quit = true;
		}
	}
	return nullptr;
}

sf::Texture MenuContext::render(const sf::Vector2f & mouse_pos) {
	text.setString("Main menu");

	rentex.clear();

	rentex.draw(text);

	rentex.display();

	return rentex.getTexture();
}
