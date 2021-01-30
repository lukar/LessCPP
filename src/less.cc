#include <less.h>


using namespace std::string_literals;

std::string get_side_text(Game const& game) {
	std::string displayText;
	if ( game.getState() == GameState::ONGOING or game.getState() == GameState::LAST_TURN ) {
		displayText += "Remaining moves: "s + std::to_string(game.moves_left()) + "\n"s;
		displayText += "Turn: "s + (game.active_player() == Player::WHITE ? "White"s : "Black"s) + "\n"s;
		displayText += "White total moves : "s + std::to_string(game.white_moves()) + "\n"s;
		displayText += "Black total moves : "s + std::to_string(game.black_moves()) + "\n"s;
	} else if (game.getState() == GameState::ENDED) {
		displayText += "GAME OVER\n"s;
		if (game.winning_player() == Player::NONE){
			displayText += "It's a tie!"s;
		} else {
			displayText += (game.winning_player() == Player::WHITE ? "White"s : "Black"s) + " wins"s;
		}
	} else if (game.getState() == GameState::PREVIEW) {
		displayText += "PREVIEW\n"s;
		displayText += "moves back: "s + std::to_string(game.m_history_index);
	}
	return displayText;
}

sf::Sound getSound(std::string const& filepath, std::deque<sf::SoundBuffer> &soundBuffers){
	soundBuffers.emplace_back();
	if (!soundBuffers.back().loadFromFile(filepath))
		throw std::runtime_error("Cannot find the sound file " + filepath);
	return sf::Sound(soundBuffers.back());
}

sf::Text initializeText(sf::Font const& font, int size, int xpos, int ypos, sf::Color color) {
	sf::Text text("", font, size);
	text.setPosition(xpos, ypos);
	text.setFillColor(color);

	return text;
}

