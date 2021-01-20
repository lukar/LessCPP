#include "sub_menu_context.h"
#include <fstream>
#include <array>

#include <chrono>
#include <ctime>
#include <iomanip>
#include "helpers.h"
#include "osdialog.h"

static std::string getTimeStr(){
	std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

	std::array<char, 30> s = {'\0'};
	std::strftime(s.data(), s.size(), "%Y-%m-%d-%H-%M-%S", std::localtime(&now));
	return std::string(s.data());
}

SubMenuContext::SubMenuContext(int preQuitLevel, sf::Texture pretext, const Game & game)
	: m_pretext(pretext), m_game(game)
{
	quitLevel = preQuitLevel + 1;

	text.setString("Pause");
}

Context* SubMenuContext::processEvent(const sf::Event & event)
{
	if (event.type == sf::Event::MouseButtonPressed) {
		if (returnButton.contains(m_mousepos)) quit = true;
		else if (continueButton.contains(m_mousepos)) { quitLevel = 1; quit = true; }
		else if (saveGameButton.contains(m_mousepos)) {
			osdialog_filters* filters = osdialog_filters_parse("json:json");
			char* filename = osdialog_file(OSDIALOG_SAVE, ".json", std::string(getTimeStr() + ".json").c_str(), filters);

			if (filename != nullptr) {
				std::ofstream of(filename);
				of << std::setw(4) << m_game.getJsonRepresentation();
				free(filename);
			}
		}
		else if (sendGameButton.contains(m_mousepos)) {
			udpSendStr(m_game.getJsonRepresentation().dump());
		}
	}
	if (event.type == sf::Event::KeyPressed) {
	}
	return nullptr;
}

sf::Texture SubMenuContext::render() {
	rentex.clear();

	rentex.draw(text);
	rentex.draw(returnButton);
	rentex.draw(continueButton);
	rentex.draw(saveGameButton);
	rentex.draw(sendGameButton);
	rentex.display();

	return rentex.getTexture();
}

