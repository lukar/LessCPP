#include "globals.h"

#include "block_shape.hh"
#include "helpers.h"
#include "player.h"
#include <SFML/Graphics.hpp>
#include <random>
#include <iostream>

Player *selected_player = nullptr;

int main() {

	std::default_random_engine eng((std::random_device()) ());
	std::uniform_int_distribution<int8_t> rand0to6(0, 6);
	std::uniform_int_distribution<int8_t> rand0to3(0, 3);

	sf::RenderWindow window(sf::VideoMode(600, 600), "Less game", sf::Style::Close);
	window.setPosition(sf::Vector2i(0, 0));

	std::vector<std::vector<Block>> field;
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

	sf::Color White{255, 222, 173};
	sf::Color Black{139, 69, 19};

	std::vector<Player> whitePlayers{
			Player{player_size, White, {0, 0}},
			Player{player_size, White, {1, 0}},
			Player{player_size, White, {0, 1}},
			Player{player_size, White, {1, 1}}
	};

	std::vector<Player> blackPlayers{
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
					selected_player->setLocation(getLocationFromPosition(selected_player->getPosition()));
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
