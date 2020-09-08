#include "less.h"


using namespace std::string_literals;

std::string get_side_text(Game const& game) {
	std::string displayText;
	if ( game.getState() != State::ENDED ) {
		displayText += "Remaining moves: "s + std::to_string(game.moves_left()) + "\n"s;
		displayText += "Turn: "s + (game.active_side() == Side::WHITE ? "White"s : "Black"s) + "\n"s;
		displayText += "White total moves : "s + std::to_string(game.white_moves()) + "\n"s;
		displayText += "Black total moves : "s + std::to_string(game.black_moves()) + "\n"s;
	} else {
		displayText += "GAME OVER\n"s;
		if (game.winning_side() == Side::NONE){
			displayText += "It's a tie!"s;
		} else {
			displayText += (game.winning_side() == Side::WHITE ? "White"s : "Black"s) + " wins"s;
		}
	}
	return displayText;
}

sf::Sound getSound(std::string const& filepath, std::deque<sf::SoundBuffer> &soundBuffers){
	soundBuffers.emplace_back();
	if (!soundBuffers.back().loadFromFile(filepath))
		throw std::runtime_error("Cannot find the sound file " + filepath);
	return sf::Sound(soundBuffers.back());
}

sf::Font getFont(std::string const& filepath) {
	sf::Font font;
	if (!font.loadFromFile(filepath)) {
		throw std::runtime_error("Cannot find the font file " + filepath);
	}
	return font;
}

sf::Text initializeSideText(sf::Font const& font, int xpos, int ypos, sf::Color color) {
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(10);
	text.setPosition(xpos, ypos);
	text.setFillColor(color);

	return text;
}

