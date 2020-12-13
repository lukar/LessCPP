#include "main_menu_context.h"
#include "game_context.h"
#include "wall.h"

#include "ai.h"
#include "helpers.h"

#include <fstream>
#include "osdialog.h"
#include "nlohmann/json.hpp"

MainMenuContext::MainMenuContext() {
	rentex.create(window_width, window_height);
}

Context* MainMenuContext::update(const sf::Event & event, const sf::Vector2f & mouse_pos)
{
	if (event.type == sf::Event::MouseButtonPressed) {
		if (quitButton.contains(mouse_pos)) quit = true;
		else if (startGameButton.contains(mouse_pos)) return new GameContext(wall::generateNwallconfigs<9>());
		else if (loadGameButton.contains(mouse_pos)) {
			osdialog_filters* filters = osdialog_filters_parse("json:json");
			char* filename = osdialog_file(OSDIALOG_OPEN, ".json", "Select a json file", filters);

			if (filename != nullptr) {
				std::ifstream game_file(filename);
				nlohmann::json game_json{};
				
				game_file >> game_json;

				auto * tmp = new GameContext(game_json);
				free(filename);
				return tmp;
			}
		};
	}
	if (event.type == sf::Event::KeyPressed) {
	}
	return nullptr;
}

sf::Texture MainMenuContext::render([[maybe_unused]] const sf::Vector2f & mouse_pos) {
	text.setString("Less Game");

	rentex.clear();

	rentex.draw(text);
	rentex.draw(quitButton);
	rentex.draw(startGameButton);
	rentex.draw(loadGameButton);
	

	rentex.display();

	return rentex.getTexture();
}

