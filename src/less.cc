#include "const_globals.h"
#include "player.h"
#include "block_shape.h"
#include "helpers.h"
#include "field.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <random>
#include <cassert>

using namespace std::string_literals;

int main() {

	sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Less game", sf::Style::Close);

	window.setPosition(sf::Vector2i(0, 0));

    // create game field
    auto field = Field(&window);

    // load game sounds
    sf::SoundBuffer buffer_pickup;
    if (!buffer_pickup.loadFromFile("../sounds/sfx_menu_move2.wav"))
        throw std::runtime_error("Cannot find the sound file '../sounds/sfx_menu_move2.wav'");

    sf::SoundBuffer buffer_drop;
    if (!buffer_drop.loadFromFile("../sounds/sfx_menu_move3.wav"))
        throw std::runtime_error("Cannot find the sound file '../sounds/sfx_menu_move3.wav'");

    sf::SoundBuffer buffer_illegal;
    if (!buffer_illegal.loadFromFile("../sounds/sfx_sounds_error10.wav"))
        throw std::runtime_error("Cannot find the sound file '../sounds/sfx_sounds_error10.wav'");

    sf::Sound sound_drop, sound_pickup, sound_illegal;
    sound_drop.setBuffer(buffer_drop);
    sound_pickup.setBuffer(buffer_pickup);
    sound_illegal.setBuffer(buffer_illegal);

	// generate side text
    sf::Text text;
    sf::Font font;
    if (!font.loadFromFile("../resources/Roboto_Medium.ttf")) {
        if (!font.loadFromFile("resources/Roboto_Medium.ttf")) {
            throw std::runtime_error("Cannot find the font file 'resources/Roboto_Medium.ttf'");
        }
    }
    text.setFont(font);
    text.setCharacterSize(10);
    text.setPosition(window_height + 10, 10);
    text.setFillColor(sf::Color::Green);

	while (window.isOpen()) {

		std::string displayText;
        if ( !field.game_over() ) {
            displayText += "Remaining moves: "s + std::to_string(field.moves_left()) + "\n"s;
            displayText += "Turn: "s + (field.active_side() == White ? "White"s : "Black"s) + "\n"s;
            displayText += "White total moves : "s + std::to_string(field.white_moves()) + "\n"s;
            displayText += "Black total moves : "s + std::to_string(field.black_moves()) + "\n"s;
		} else {
			displayText += "GAME OVER\n";
            if (field.winning_side() == Noone){
                displayText += "It's a tie!"s;
            } else {
                displayText += (field.winning_side() == White ? "White"s : "Black"s) + " wins"s;
            }
		}
		text.setString(displayText);

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
            if (!field.game_over()) {
				// GRAB PLAYER
				if (event.type == sf::Event::MouseButtonPressed) {
                    for (auto &player : field.active_players()) {
						if (euclideanDistance(getMousePosition(window), player.getPosition()) <= player_size) {
                            sound_pickup.play();
                            field.selectPlayer(player);
						}
					}
				}
				// DROP PLAYER
				if (event.type == sf::Event::MouseButtonReleased) {
                    if (field.existsPlayerSelected()) {
                        std::optional<Location> new_location = getMouseLocation(window);

                        if (new_location){
                            if (field.moveSelectedPlayer(new_location.value())){
                                sound_drop.play();
                            } else {
                                sound_illegal.play();
                            }
                        } else {
                            sound_illegal.play();
                            field.unselectPlayer();
                        }
					}
				}
			}
		}


		// DRAWING
		window.clear();

        window.draw(field);

		window.draw(text);

		window.display();

	}

	return 0;
}
