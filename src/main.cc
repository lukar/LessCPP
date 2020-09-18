#include "const_globals.h"
#include "wall.h"
#include "helpers.h"
#include "game.h"
#include "gui.h"
#include "less.h"
#include "ai.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <algorithm>
#include <chrono>
#include <thread>
#include <set>

#include <iostream>
#include <deque>


int main() {

	sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Less game", sf::Style::Close);
	window.setPosition(sf::Vector2i(0, 0));
	window.setFramerateLimit(30);

	auto wall_configs = wall::generateNwallconfigs<9>();

	Game game = Game(wall_configs);
	Gui gui = Gui(wall_configs);

	Piece * clicked_piece = nullptr;

	// load game sounds
	std::deque<sf::SoundBuffer> soundBuffers;

	sf::Sound sound_pickup = getSound("sounds/sfx_menu_move2.wav", soundBuffers);
	sf::Sound sound_drop = getSound("sounds/sfx_menu_move3.wav", soundBuffers);
	sf::Sound sound_illegal = getSound("sounds/sfx_sounds_error10.wav", soundBuffers);

	// generate side text
	sf::Font font = getFont("resources/Roboto_Medium.ttf");
	sf::Text text = initializePlayerText(font, window_height + 10, 10, sf::Color::Green);

	while (window.isOpen()) {
		text.setString(get_side_text(game));

		auto event = sf::Event();
		while (window.pollEvent(event)) {
			// ON CLOSE EVENT
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			// CLOSE WHEN PRESSED Q
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Q) {
					window.close();
				}
			}
			// HUMAN
			if ( game.getState() != State::ENDED and game.active_player() == Player::WHITE ) {
				// GRAB PLAYER
				if (event.type == sf::Event::MouseButtonPressed) {
					for (auto &piece : gui.getPieces(game.active_player())) {
						if (euclideanDistance(getMousePosition(window), piece.getPosition()) <= piece_size) {
							sound_pickup.play();
							clicked_piece = &piece;
						}
					}
				}
				// DROP PLAYER
				if (event.type == sf::Event::MouseButtonReleased) {
					if (clicked_piece != nullptr) {
						std::optional<Location> new_location = getMouseLocation(window);

						if (new_location){
							if (game.movePiece(clicked_piece->getLocation(), new_location.value())){
								clicked_piece->setLocation(new_location.value());
								sound_drop.play();
							} else {
								sound_illegal.play();
							}
						} else {
							sound_illegal.play();
						}
						clicked_piece->resetPosition();
						clicked_piece = nullptr;
					}
				}
			}
			// AI
			else if ( game.getState() != State::ENDED and game.active_player() == Player::BLACK ) {
				auto path = recurseFindOptimal(game, Player::BLACK, 1, 0, 100, evaluation(game)).value();
				for( auto elem: path) {
					if (game.active_player() != Player::BLACK) break;
					auto newLocation = game.movePiece(std::get<0>(elem), std::get<1>(elem));
					gui.getPieces(Player::BLACK)[static_cast<uint>(std::get<0>(elem))].setLocation(newLocation.value());
					gui.getPieces(Player::BLACK)[static_cast<uint>(std::get<0>(elem))].resetPosition();
					window.clear();
					window.draw(text);
					window.draw(gui);
					window.display();
				}
			}
		}


		// DRAWING
		window.clear();

		if (clicked_piece != nullptr) clicked_piece->setPosition(getMousePosition(window));

		window.draw(text);

		window.draw(gui);
		if (clicked_piece != nullptr) window.draw(*clicked_piece);

		window.display();



	}

	return 0;
}
