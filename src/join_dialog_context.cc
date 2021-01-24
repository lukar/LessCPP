#include "join_dialog_context.h"
#include "main_menu_context.h"
#include "game_context.h"
#include "helpers.h"
#include <iostream>
#include "osdialog.h"
#include <fstream>
#include <sstream>

Context* JoinDialogContext::processBackgroundTask() { return nullptr; }
Context* JoinDialogContext::processEvent(const sf::Event & event)
{
	if (event.type == sf::Event::MouseButtonPressed) {
		if (quitButton.contains(m_mousepos)) quit = true;
		else if (IPTextInput.contains(m_mousepos)); // do nothing
	}
	if (event.key.code == sf::Keyboard::BackSpace)
		IPTextInput.backspace();
	if (event.key.code == sf::Keyboard::Enter) {
		ip_player2 = IPTextInput.getText();
		return join_game();
	}
	if (event.type == sf::Event::TextEntered)
	{
		if (event.text.unicode < 128)
			IPTextInput.append(static_cast<char>(event.text.unicode));
	}
	return nullptr;
}

Context* JoinDialogContext::join_game() {
	std::string json_string = get_game_tcp_packets(ip_player2, tcp_port);
	std::stringstream ss;
	ss << json_string;
	nlohmann::json game_json{};
	ss >> game_json;
	return new GameContext(game_json, ip_player2, tcp_port);
}

sf::Texture JoinDialogContext::render() {
	text.setString("Less Game");

	rentex.clear();

	rentex.draw(IPTextInput);
	rentex.draw(quitButton);

	rentex.display();

	return rentex.getTexture();
}