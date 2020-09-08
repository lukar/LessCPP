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

enum class STATE { MAIN_MENU, IN_GAME, PAUSED, BLOCKING, QUIT };

struct Sounds {
public:
	sf::Sound pickup;
	sf::Sound drop;
	sf::Sound illegal;

	Sounds(std::string path_pickup, std::string path_drop, std::string path_illegal) {
		 pickup = getSound(path_pickup, soundBuffers);
		 drop = getSound(path_drop, soundBuffers);
		 illegal = getSound(path_illegal, soundBuffers);
	}
private:
	std::deque<sf::SoundBuffer> soundBuffers;
};

enum STATE in_game(sf::Vector2<float> mouse_pos, sf::Event & event, Game & game, Gui & gui, Sounds & sounds, sf::Text & text) {
	if ( game.getState() != State::ENDED ) { //  and game.active_side() == Side::WHITE
		// GRAB PLAYER
		if (event.type == sf::Event::MouseButtonPressed) {
			for (auto &player : gui.getPlayers(game.active_side())) {
				if (euclideanDistance(mouse_pos, player.getPosition()) <= player_size) {
					sounds.pickup.play();
					gui.selected_player = &player;
				}
			}
		}
		// DROP PLAYER
		if (event.type == sf::Event::MouseButtonReleased) {
			if (gui.selected_player != nullptr) {
				std::optional<Location> new_location = getMouseLocation(mouse_pos);

				if (new_location){
					if (game.movePlayer(gui.selected_player->getLocation(), new_location.value())){
						gui.selected_player->setLocation(new_location.value());
						sounds.drop.play();
					} else {
						sounds.illegal.play();
					}
				} else {
					sounds.illegal.play();
				}
				gui.selected_player->resetPosition();
				gui.selected_player = nullptr; // unselect player
			}
		}
//	}
//	else if ( game.getState() != State::ENDED and game.active_side() == Side::BLACK ) {
//		auto path = recurseFindOptimal(game, Side::BLACK, 1, 0, 100, evaluation(game)).value();
//		for( auto elem: path) {
//			if (game.active_side() != Side::BLACK) break;
//			auto newLocation = game.movePlayer(std::get<0>(elem), std::get<1>(elem));
//			gui.getPlayers(Side::BLACK)[static_cast<uint>(std::get<0>(elem))].setLocation(newLocation.value());
//			gui.getPlayers(Side::BLACK)[static_cast<uint>(std::get<0>(elem))].resetPosition();
//			return STATE::BLOCKING;
//		}
	}

	if (gui.selected_player != nullptr) gui.selected_player->setPosition(mouse_pos);

	return STATE::IN_GAME;

}

int main() {

	auto currentState = STATE::IN_GAME;
	auto lastState = STATE::IN_GAME;

	sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Less game", sf::Style::Close);
	window.setPosition(sf::Vector2i(0, 0));
	window.setFramerateLimit(30);

	auto wall_configs = wall::generateNwallconfigs<9>();

	Game game = Game(wall_configs);
	Gui gui = Gui(wall_configs);


	// load game sounds
	Sounds sounds = Sounds("sounds/sfx_menu_move2.wav", "sounds/sfx_menu_move3.wav", "sounds/sfx_sounds_error10.wav");

	// generate side text
	sf::Font font = getFont("resources/Roboto_Medium.ttf");
	sf::Text text = initializeSideText(font, window_height + 10, 10, sf::Color::Green);

	while (currentState != STATE::QUIT) {
		text.setString(get_side_text(game));

		auto event = sf::Event();

		while(window.pollEvent(event)){

			// CLOSE WHEN PRESSED Q
			if (event.type == sf::Event::KeyPressed and event.key.code == sf::Keyboard::Q) {
				currentState = STATE::QUIT;
			}
			// ON CLOSE EVENT
			if (event.type == sf::Event::Closed) {
				currentState = STATE::QUIT;
			}

			switch( currentState ) {
				case STATE::IN_GAME:
					lastState = currentState;
					currentState = in_game(getMousePosition(window), event, game, gui, sounds, text);
					break;

				case STATE::QUIT:
					window.close();
					break;

				case STATE::BLOCKING:
					std::this_thread::sleep_for(std::chrono::seconds(1));
					currentState = lastState;
					lastState = STATE::BLOCKING;
					break;

				default:
					std::cout << "Error: Should never be here. The state is undefined" << std::endl;
					break;
			}

		}


		// DRAWING
		window.clear();

		window.draw(text);

		window.draw(gui);

		window.display();


	}

	return 0;
}
