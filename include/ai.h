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

constexpr int locationDistance(Location a, Location b) {
	return cabs(a.x - b.x) + cabs(a.y - b.y);
}

constexpr int piecesApart(std::array<Location, 4> pieces) {
	int result = 0;
	for (const auto & a : pieces) {
		for (const auto & b : pieces) {
			if ( a == b ) continue;
			result += locationDistance(a, b);
		}
	}
	return result - 16;
}

constexpr int evaluation(const GameRef & game) {
	const auto whites = game.getPieces(Player::WHITE);
	const auto blacks = game.getPieces(Player::BLACK);
	int eval = 0;
	for (size_t i = 0; i < 4; ++i) {
		eval += sumLocation(whites[i] + blacks[i]);
	}
	eval -= piecesApart(whites)/4;
	eval += piecesApart(blacks)/4;
	if (game.winning_player() == Player::WHITE) eval += 10;
	else if (game.winning_player() == Player::BLACK) eval -= 10;
	return eval;
}

Path recurseFindOptimal(const GameRef, const Player, int, int, int);




#endif // AI_H
