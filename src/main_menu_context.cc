#include "main_menu_context.h"


MainMenuContext::MainMenuContext() {
	rentex.create(window_width, window_height);
}

Context* MainMenuContext::update(const sf::Event & event, const sf::Vector2f & mouse_pos)
{
	if (event.type == sf::Event::MouseButtonPressed) {
		if (quitButton.contains(mouse_pos)) quit = true;
		else if (startGameButton.contains(mouse_pos)) return new GameContext();
	}
	if (event.type == sf::Event::KeyPressed) {
	}
	return nullptr;
}

sf::Texture MainMenuContext::render(const sf::Vector2f & mouse_pos) {
	text.setString("Less Game");

	rentex.clear();

	rentex.draw(text);
	rentex.draw(quitButton);
	rentex.draw(startGameButton);
	

	rentex.display();

	return rentex.getTexture();
}

