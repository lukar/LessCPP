#include "main_menu_context.h"
#include "game_context.h"
#include "wall.h"

#include "ai.h"
#include "helpers.h"

#include <sstream>
#include <fstream>
#include "osdialog.h"
#include "nlohmann/json.hpp"
#include <iostream>

Context* MainMenuContext::processBackgroundTask() { return nullptr; }
Context* MainMenuContext::processEvent(const sf::Event & event)
{
	if (event.type == sf::Event::MouseButtonPressed) {
		if (quitButton.contains(m_mousepos)) quit = true;
		else if (startGameButton.contains(m_mousepos)) return new GameContext(wall::generateNwallconfigs<9>());
		else if (loadGameButton.contains(m_mousepos)) {
			osdialog_filters* filters = osdialog_filters_parse("json:json");
			char* filename = osdialog_file(OSDIALOG_OPEN, ".json", "Select a json file", filters);

			if (filename != nullptr) {
				std::ifstream game_file(filename);
				nlohmann::json game_json{};
				
				game_file >> game_json;
				auto* tmp = new GameContext(game_json);
				free(filename);
				return tmp;
			}
		};
	}
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::J) {
			return join_game();
		}
	}


	return nullptr;
}

Context* MainMenuContext::join_game(void) {
	std::string json_string;
	json_string = get_game_tcp_packets(ip_player2, tcp_port);
	std::stringstream ss;
	ss << json_string;
	nlohmann::json game_json{};
	ss >> game_json;
	auto* tmp = new GameContext(game_json, ip_player2, tcp_port);
	return tmp;
}

sf::Texture MainMenuContext::render() {
	text.setString("Less Game");

	rentex.clear();

	rentex.draw(text);
	rentex.draw(quitButton);
	rentex.draw(startGameButton);
	rentex.draw(loadGameButton);
	

	rentex.display();

	return rentex.getTexture();
}

