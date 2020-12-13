#include "sub_menu_context.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <array>
#include <chrono>
#include <ctime>
#include <iomanip>

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
	rentex.create(window_width, window_height);

	text.setString("Pause");
}

Context* SubMenuContext::update(const sf::Event & event, const sf::Vector2f & mouse_pos)
{
	if (event.type == sf::Event::MouseButtonPressed) {
		if (returnButton.contains(mouse_pos)) quit = true;
		else if (continueButton.contains(mouse_pos)) { quitLevel = 1; quit = true; }
		else if (saveGameButton.contains(mouse_pos)) {
			std::ofstream of(getTimeStr() + ".json");
			of << std::setw(4) << m_game.getJsonRepresentation();
		}
	}
	if (event.type == sf::Event::KeyPressed) {
	}
	return nullptr;
}

sf::Texture SubMenuContext::render(const sf::Vector2f & mouse_pos) {
	rentex.clear();

	rentex.draw(text);
	rentex.draw(returnButton);
	rentex.draw(continueButton);
	rentex.draw(saveGameButton);

	rentex.display();

	return rentex.getTexture();
}

