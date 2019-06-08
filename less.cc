#include "globals.h"

#include "block_shape.h"
#include "helpers.h"
#include "player.h"
#include <SFML/Graphics.hpp>
#include <random>
#include <iostream>


Player *selected_player = nullptr;

std::vector<std::vector<Block>> field;

sf::Color White;
sf::Color Black;

std::vector<Player> whitePlayers;

std::vector<Player> blackPlayers;


int moves = 3;

int moveCost(Location old_l, Location new_l) {
	for (const auto &player : whitePlayers) {
		if (player.getLocation() == new_l) return 0;
	}
	for (const auto &player : blackPlayers) {
		if (player.getLocation() == new_l) return 0;
	}
	if (new_l.first - old_l.first != 0 && new_l.second - old_l.second != 0) return 0;
	if (abs(new_l.first - old_l.first) + abs(new_l.second - old_l.second) > 1) {

	}


	return 1;
}


int main() {

	std::default_random_engine eng((std::random_device()) ());
	std::uniform_int_distribution<int8_t> rand0to6(0, 6);
	std::uniform_int_distribution<int8_t> rand0to3(0, 3);

	sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Less game", sf::Style::Close);
	window.setPosition(sf::Vector2i(0, 0));

	for (int y = 0; y < 3; ++y) {
		std::vector<Block> tmp;
		for (int x = 0; x < 3; ++x) {
			Block tmp_block{block_size, block_border, rand0to6(eng)};
			tmp_block.setPosition(x * block_size + block_size / 2, y * block_size + block_size / 2);
			tmp_block.setRotation(rand0to3(eng) * 90);
			tmp.emplace_back(tmp_block);
		}
		field.emplace_back(tmp);
	}

	White = {255, 222, 173};
	Black = {139, 69, 19};

	whitePlayers = {
					Player{player_size, White, {0, 0}},
					Player{player_size, White, {1, 0}},
					Player{player_size, White, {0, 1}},
					Player{player_size, White, {1, 1}}
	};

	blackPlayers = {
					Player{player_size, Black, {4, 4}},
					Player{player_size, Black, {5, 4}},
					Player{player_size, Black, {4, 5}},
					Player{player_size, Black, {5, 5}}
	};

	while (window.isOpen()) {
		sf::Event event{};
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Q) {
					window.close();
				}
			}
			if (event.type == sf::Event::MouseButtonPressed) {
				for (auto &player : whitePlayers) {
					if (euclideanDistance(getMousePosition(window), player.getPosition()) <= player_size) {
						selected_player = &player;
						selected_player->setSelected();
					}
				}
				for (auto &player : blackPlayers) {
					if (euclideanDistance(getMousePosition(window), player.getPosition()) <= player_size) {
						selected_player = &player;
						selected_player->setSelected();
					}
				}
			}
			if (event.type == sf::Event::MouseButtonReleased) {
				if (selected_player) {
					Location new_location = getMouseLocation(window).value_or(selected_player->getLocation());
					Location old_location = selected_player->getLocation();
					if (moveCost(old_location, new_location)) {
						selected_player->setPosition(new_location);
						selected_player->setLocation(new_location);
					} else {
						selected_player->setPosition(old_location);
					}
					selected_player->unsetSelected();
					selected_player = nullptr;
				}

			}
		}

		window.clear();

		for (auto &row: field) {
			for (auto &block: row) {
				window.draw(block);
			}
		}
		for (auto &player: whitePlayers) {
			if (player.isSelected()) continue;
			window.draw(player);
		}
		for (auto &player: blackPlayers) {
			if (player.isSelected()) continue;
			window.draw(player);
		}
		if (selected_player) {
			selected_player->setPosition(getMousePosition(window));
			window.draw(*selected_player);
		}

		window.display();
	}

	return 0;
}
