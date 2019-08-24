#include "const_globals.h"
#include "player.h"
#include "block_shape.h"
#include "helpers.h"

#include <SFML/Graphics.hpp>
#include <random>
#include <cassert>

using namespace std::string_literals;

std::vector<std::vector<Block>> field;

std::vector<Player> whitePlayers;
std::vector<Player> blackPlayers;

sf::Color const White = {255, 222, 173};
sf::Color const Black = {139, 69, 19};

Player * selected_player = nullptr;

struct GameState {
private:
    int moves_left = 3;
    bool game_over = false;
    sf::Color active_side = White;
    int white_moves = 0;
    int black_moves = 0;
public:
    GameState () = default;
    [[nodiscard]] int getWhiteMoves() const { return white_moves; }

    [[nodiscard]] int getBlackMoves() const { return black_moves; }
    [[nodiscard]] sf::Color getActiveSide() const { return active_side; }
    [[nodiscard]] int getMovesLeft() const { return moves_left; }
    [[nodiscard]] bool isGameOver() const { return game_over; }

    bool move_piece(int cost) {
        if ( cost == 0 || cost > moves_left ) return false;
        (active_side == White ? white_moves : black_moves) += cost;
        moves_left -= cost;
        if ( moves_left == 0 ) {
            next_turn();
            moves_left = 3;
        }
        return true;
    }

    void next_turn() { active_side = (active_side == White ? Black : White); }

};

int countInnerWalls(Location const start, Location const end) {

	// differences
	int const dx = end[0] - start[0];
	int const dy = end[1] - start[1];

	// block coordinates
	int const sx = start[0] / 2;
	int const sy = start[1] / 2;

	int const ex = end[0] / 2;
	int const ey = end[1] / 2;
	Block bstart = field[sy][sx];
	Block bend = field[ey][ex];

	// blocks are orthogonal and at most one space apart
	assert((dx == 0 && abs(dy) == 1) || (abs(dx) == 1 && dy == 0));

	int const first = (dx ? 1 : -1);
	int const second = (sx == ex && sy == ey ? 0 : dx + dy);
	int const third = -1 + 2 * abs(dx) * (start[1] % 2) + 2 * abs(dy) * (start[0] % 2);

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
	if (newL[0] - oldL[0] != 0 && newL[1] - oldL[1] != 0) return 0;

	if (isPlayerAtLocation(newL)) return 0;

	int wallCount = 0;
	int distance = abs(newL[0] - oldL[0]) + abs(newL[1] - oldL[1]);
	if (distance > 2) return 0;
	if (distance == 2) {
		Location const connecting = {(newL[0] + oldL[0]) / 2, (newL[1] + oldL[1]) / 2};
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

bool playersInBlock(std::vector<Player> const &team, std::vector<Location> const &locations) {

}

sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Less game", sf::Style::Close);


int main() {

	// Game initialization
	std::default_random_engine eng((std::random_device()) ());
	std::uniform_int_distribution<int8_t> rand0to6(0, 6);
	std::uniform_int_distribution<int8_t> rand0to3(0, 3);

	window.setPosition(sf::Vector2i(0, 0));

	// create default game state
	auto game = GameState();

	// generate the field
	for (int y = 0; y < 3; ++y) {
		std::vector<Block> tmp;
		for (int x = 0; x < 3; ++x) {
			Block tmp_block{block_size, block_border, wall::wall_configs[rand0to6(eng)], rand0to3(eng)};
			tmp_block.setPosition(x * block_size + block_size / 2, y * block_size + block_size / 2);
			tmp.emplace_back(tmp_block);
		}
		field.emplace_back(tmp);
	}

	// generate players
	for( auto & start: whiteStart) {
	    whitePlayers.emplace_back(Player(player_size, White, start));
	}

	for( auto & start: blackStart) {
	    blackPlayers.emplace_back(Player(player_size, Black, start));
	}

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
		displayText += "Remaining moves: "s + std::to_string(game.getMovesLeft()) + "\n"s;
		displayText += "Turn: "s + (game.getActiveSide() == White ? "White"s : "Black"s) + "\n"s;
		displayText += "White total moves : "s + std::to_string(game.getWhiteMoves()) + "\n"s;
		displayText += "Black total moves : "s + std::to_string(game.getBlackMoves()) + "\n"s;
		text.setString(displayText);

		sf::Event event{};
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
			if (!game.isGameOver()) {
				// GRAB PLAYER
				if (event.type == sf::Event::MouseButtonPressed) {
					if (game.getActiveSide() == White) {
						for (auto &player : whitePlayers) {
							if (euclideanDistance(getMousePosition(), player.getPosition()) <= player_size) {
								selected_player = &player;
								selected_player->setSelected();
							}
						}
					} else {
						for (auto &player : blackPlayers) {
							if (euclideanDistance(getMousePosition(), player.getPosition()) <= player_size) {
								selected_player = &player;
								selected_player->setSelected();
							}
						}
					}
				}
				// DROP PLAYER
				if (event.type == sf::Event::MouseButtonReleased) {
					if (selected_player) {
						Location new_location = getMouseLocation().value_or(selected_player->getLocation());
						Location old_location = selected_player->getLocation();
						int cost = moveCost(old_location, new_location);

						// move player to new location if move_piece was successful
						selected_player->setLocation(game.move_piece(cost) ? new_location : old_location);
						selected_player->unsetSelected();
						selected_player = nullptr;
					}

				}
			}
		}

		// DRAWING
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
			selected_player->setPosition(getMousePosition());
			window.draw(*selected_player);
		}

		window.draw(text);

		window.display();

	}

	return 0;
}
