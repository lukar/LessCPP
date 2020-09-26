#ifndef AI_H
#define AI_H

#include "game.h"

#include <vector>
#include <tuple>
#include <optional>

// piece, direction, evaluation
typedef std::tuple<int, Direction, int> Move;
typedef std::vector<Move> Path;



constexpr int sumLocation(Location loc) {
	return loc.x + loc.y;
}


constexpr int evaluation(const Game & game) {
	const auto whites = game.getPieces(Player::WHITE);
	const auto blacks = game.getPieces(Player::BLACK);
	int eval = 0;
	for (size_t i = 0; i < 4; ++i) {
		eval += sumLocation(whites[i]);
		eval += sumLocation(blacks[i]);
		eval += std::min(whites[i].x, whites[i].y)*2; // White wants to middle (wants to maximize)
		eval += std::max(blacks[i].x, blacks[i].y)*2; // Black wants to middle (wants to minimize)
	}
	if (piecesInLocations(whites, blackStart)) eval += 900;
	else if (piecesInLocations(blacks, whiteStart)) eval -= 1000;
	return eval;
}

Path recurseFindOptimal(const Game, const Player, int, int, int, int);


int alphaBeta(const Game state, int node, int depth, int alpha, int beta);
std::vector<Move> findOptimalMove(Game state, int depth);



#endif // AI_H
