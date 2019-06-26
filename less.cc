#include "const_globals.h"
#include "player.h"
#include "block_shape.h"
#include "helpers.h"

#include <SFML/Graphics.hpp>
#include <random>
#include <iostream>
#include <cassert>


Player *selected_player = nullptr;

std::vector<std::vector<Block>> field;

std::vector<Player> whitePlayers;
std::vector<Player> blackPlayers;

sf::Color const White = {255, 222, 173};
sf::Color const Black = {139, 69, 19};

int moves = 3;

struct Turn {
private:
		sf::Color m_current;
public:
		explicit Turn(sf::Color firstToMove) : m_current(firstToMove) {}

		bool operator==(const sf::Color &rhs) const {
			return m_current == rhs;
		}

		bool operator!=(const sf::Color &rhs) const {
			return !(m_current == rhs);
		}

		void toggle() { m_current = (m_current == White ? Black : White); }
};

Turn turn{White};

int countInnerWalls(Location const start, Location const end) {

	// differences
	int const dx = end.first - start.first;
	int const dy = end.second - start.second;

	// block coordinates
	int const sx = start.first / 2;
	int const sy = start.second / 2;

	int const ex = end.first / 2;
	int const ey = end.second / 2;

	Block bstart = field[sy][sx];
	Block bend = field[ey][ex];

	// blocks are orthogonal and at most one space apart
	assert((dx == 0 && abs(dy) == 1) || (abs(dx) == 1 && dy == 0));

	int const first = (dx ? 1 : -1);
	int const second = (sx == ex && sy == ey ? 0 : dx + dy);
	int const third = -1 + 2 * abs(dx) * (start.second % 2) + 2 * abs(dy) * (start.first % 2);

	int N = 0;
	if (second) {
		N += bend.hasWall({first, -second, third});
	}
	N += bstart.hasWall({first, second, third});

	return N;
}

bool isPlayerAtLocation(Location const location) {
	bool toReturn = false;
	for (const auto &player : whitePlayers) {
		if (player.getLocation() == location) toReturn = true;
	}
	for (const auto &player : blackPlayers) {
		if (player.getLocation() == location) toReturn = true;
	}
	return toReturn;
}

// return 0 means that it's impossible to make that move
int moveCost(Location oldL, Location newL) {
	if (newL.first - oldL.first != 0 && newL.second - oldL.second != 0) return 0;

	if (isPlayerAtLocation(newL)) return 0;

	int wallCount = 0;
	int distance = abs(newL.first - oldL.first) + abs(newL.second - oldL.second);
	if (distance > 2) return 0;
	if (distance == 2) {
		Location const connecting = {(newL.first + oldL.first) / 2, (newL.second + oldL.second) / 2};
		wallCount += countInnerWalls(oldL, connecting);
		wallCount += countInnerWalls(connecting, newL);

		if (!isPlayerAtLocation(connecting)) return 0;
		if (wallCount != 0) return 0;
		return 1;
	} else {
		wallCount += countInnerWalls(oldL, newL);
		return wallCount + 1;
	}


	return 0;
}

int main() {

	// Game initialization
	std::default_random_engine eng((std::random_device()) ());
	std::uniform_int_distribution<int8_t> rand0to6(0, 6);
	std::uniform_int_distribution<int8_t> rand0to3(0, 3);

	sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Less game", sf::Style::Close);
	window.setPosition(sf::Vector2i(0, 0));

	for (int y = 0; y < 3; ++y) {
		std::vector<Block> tmp;
		for (int x = 0; x < 3; ++x) {
			Block tmp_block{block_size, block_border, wall::wall_configs[rand0to6(eng)], rand0to3(eng)};
			tmp_block.setPosition(x * block_size + block_size / 2, y * block_size + block_size / 2);
			tmp.emplace_back(tmp_block);
		}
		field.emplace_back(tmp);
	}


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

	sf::Text text;
	sf::Font font;
	if (!font.loadFromFile("../resources/Roboto_Medium.ttf")) {
		throw std::runtime_error("Cannot find the font file 'resources/Roboto_Medium.ttf'");
	}
	text.setFont(font);
	text.setCharacterSize(10);
	text.setPosition(window_height + 10, 10);
	text.setFillColor(sf::Color::Green);

	while (window.isOpen()) {
		text.setString("Remaining moves: " + std::to_string(moves) + "\nTurn: " + (turn == White ? "White" : "Black"));

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
				if (turn == White) {
					for (auto &player : whitePlayers) {
						if (euclideanDistance(getMousePosition(window), player.getPosition()) <= player_size) {
							selected_player = &player;
							selected_player->setSelected();
						}
					}
				} else {
					for (auto &player : blackPlayers) {
						if (euclideanDistance(getMousePosition(window), player.getPosition()) <= player_size) {
							selected_player = &player;
							selected_player->setSelected();
						}
					}
				}
			}
			if (event.type == sf::Event::MouseButtonReleased) {
				if (selected_player) {
					Location new_location = getMouseLocation(window).value_or(selected_player->getLocation());
					Location old_location = selected_player->getLocation();
					int cost = moveCost(old_location, new_location);
					if (cost && cost <= moves) {
						moves -= cost;
						selected_player->setPosition(new_location);
						selected_player->setLocation(new_location);
					} else {
						selected_player->setPosition(old_location);
					}
					selected_player->unsetSelected();
					selected_player = nullptr;
					if (moves == 0) {
						turn.toggle();
						moves = 3;
					}
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

		window.draw(text);

		window.display();

		countInnerWalls(Location{2, 3}, Location{2, 4});

	}

	return 0;
}
