#ifndef AI_H
#define AI_H

#include "game.h"
#include "const_globals.h"

#include <vector>
#include <utility>
#include <optional>



// tuple (moves, evaluation)
using EvalPath = std::pair<Path, int>;


constexpr int sumLocation(const Location& loc) {
	return loc.x + loc.y;
}

constexpr int locationDistance(const Location& a, const Location& b) {
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

EvalPath recurseFindOptimal(const GameRef, const Player, int, int, int);




#endif // AI_H
