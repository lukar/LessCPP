#include "const_globals.h"
#include "wall.h"
#include "helpers.h"
#include "game.h"
#include "gui.h"
#include <algorithm>
#include <chrono>
#include <thread>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace std::string_literals;

uint sumPlayers(uint sum, Location player) {
    return sum + player.x + player.y;
}

typedef std::tuple<uint, Direction, uint> Path;
std::vector<Path> recurseFindOptimal(const Game state) {
    std::vector<std::vector<Path>> paths;

    for (uint player = 0; player < 4; ++player) {
        Direction direction = Direction::UP;
        do {
            Game newstate = state;
            if(auto location = newstate.movePlayer(player, direction)){
                auto players = newstate.getPlayers(Side::BLACK);
                if (newstate.active_side() == Side::BLACK) {
                    auto aux = recurseFindOptimal(newstate);
                    aux.emplace(aux.begin(), std::make_tuple(player, direction, std::accumulate(players.begin(), players.end(), 0, sumPlayers)));
                    paths.push_back(aux);
                } else {
                    paths.emplace_back(std::vector<Path>{std::make_tuple(player, direction, std::accumulate(players.begin(), players.end(), 0, sumPlayers))});
                    // std::accumulate(players.begin(), players.end(), sumPlayers)
                }
            }
            ++direction;
        } while (direction != Direction::UP);
    }
    return *std::min_element(paths.begin(), paths.end(), [](std::vector<Path> a, std::vector<Path> b){ return std::get<2>(a[0]) < std::get<2>(b[0]);});
}


int main() {

	sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Less game", sf::Style::Close);
    window.setPosition(sf::Vector2i(0, 0));

    auto wall_configs = wall::generateNwallconfigs<9>();

    Game game = Game(wall_configs);
    Gui gui = Gui(wall_configs);
    Player * clicked_player = nullptr;
//    std::optional<uint> selected_player_num;

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
        if ( game.getState() != State::ENDED ) {
            displayText += "Remaining moves: "s + std::to_string(game.moves_left()) + "\n"s;
            displayText += "Turn: "s + (game.active_side() == Side::WHITE ? "White"s : "Black"s) + "\n"s;
            displayText += "White total moves : "s + std::to_string(game.white_moves()) + "\n"s;
            displayText += "Black total moves : "s + std::to_string(game.black_moves()) + "\n"s;
		} else {
			displayText += "GAME OVER\n";
            if (game.winning_side() == Side::NONE){
                displayText += "It's a tie!"s;
            } else {
                displayText += (game.winning_side() == Side::WHITE ? "White"s : "Black"s) + " wins"s;
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
            if ( game.getState() != State::ENDED and game.active_side() == Side::WHITE ) {
//                if (event.type == sf::Event::KeyPressed) {
//                    if (event.key.code == sf::Keyboard::Num1) selected_player_num = 0;
//                    if (event.key.code == sf::Keyboard::Num2) selected_player_num = 1;
//                    if (event.key.code == sf::Keyboard::Num3) selected_player_num = 2;
//                    if (event.key.code == sf::Keyboard::Num4) selected_player_num = 3;
//                    if (event.key.code == sf::Keyboard::Up and selected_player_num) game.movePlayer(selected_player_num.value(), Direction::UP);
//                    if (event.key.code == sf::Keyboard::Down and selected_player_num) game.movePlayer(selected_player_num.value(), Direction::DOWN);
//                    if (event.key.code == sf::Keyboard::Left and selected_player_num) game.movePlayer(selected_player_num.value(), Direction::LEFT);
//                    if (event.key.code == sf::Keyboard::Right and selected_player_num) {
//                        if (auto location = game.movePlayer(selected_player_num.value(), Direction::RIGHT)) {

//                        }
//                    }
//                }
                // GRAB PLAYER
				if (event.type == sf::Event::MouseButtonPressed) {
                    for (auto &player : gui.getPlayers(game.active_side())) {
                        if (euclideanDistance(getMousePosition(window), player.getPosition()) <= player_size) {
                            sound_pickup.play();
                            clicked_player = &player;
						}
					}
				}
				// DROP PLAYER
				if (event.type == sf::Event::MouseButtonReleased) {
                    if (clicked_player != nullptr) {
                        std::optional<Location> new_location = getMouseLocation(window);

                        if (new_location){
                            if (game.movePlayer(clicked_player->getLocation(), new_location.value())){
                                clicked_player->setLocation(new_location.value());
                                sound_drop.play();
                            } else {
                                sound_illegal.play();
                            }
                        } else {
                            sound_illegal.play();
                        }
                        clicked_player->resetPosition();
                        clicked_player = nullptr;
					}
				}
            }
            else if ( game.getState() != State::ENDED and game.active_side() == Side::BLACK ) {
                auto path = recurseFindOptimal(game);
                for( auto elem: path) {
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    auto newLocation = game.movePlayer(std::get<0>(elem), std::get<1>(elem));
                    gui.getPlayers(Side::BLACK)[std::get<0>(elem)].setLocation(newLocation.value());
                    gui.getPlayers(Side::BLACK)[std::get<0>(elem)].resetPosition();
                    window.clear();
                    window.draw(text);
                    window.draw(gui);
                    window.display();
                }
            }
		}


		// DRAWING
		window.clear();

        if (clicked_player != nullptr) clicked_player->setPosition(getMousePosition(window));

		window.draw(text);

        window.draw(gui);
        if (clicked_player != nullptr) window.draw(*clicked_player);

		window.display();

	}

	return 0;
}
