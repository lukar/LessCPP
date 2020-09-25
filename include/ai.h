#ifndef AI_H
#define AI_H

#include "game.h"

#include <vector>
#include <tuple>
#include <optional>

// piece, direction, evaluation
typedef std::tuple<int, Direction, int> Path; // refactor path->move



constexpr int sumLocation(Location loc) {
	return loc.x + loc.y;
}

constexpr int evaluation(const Game & game) {
	const auto whites = game.getPieces(Player::WHITE);
	const auto blacks = game.getPieces(Player::BLACK);
	int eval = 0;
	for (size_t i = 0; i < 4; ++i) {
		eval += sumLocation(whites[i])/2;
		eval += sumLocation(blacks[i]);

	}
	if (piecesInLocations(whites, blackStart)) eval += 4;
	else if (piecesInLocations(blacks, whiteStart)) eval -= 4;
	return eval;
}

std::vector<Path> recurseFindOptimal(const Game, const Player, int, int, int, int);




#endif // AI_H
