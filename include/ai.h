#ifndef AI_H
#define AI_H

#include "game.h"

#include <vector>
#include <tuple>
#include <optional>

typedef std::tuple<int, Direction, int> Path;


constexpr int sumLocation(Location loc) {
	return loc.x + loc.y;
}

constexpr int evaluation(const Game & game) {
	const auto whites = game.getPlayers(Side::WHITE);
	const auto blacks = game.getPlayers(Side::BLACK);
	int eval = 0;
	for (size_t i = 0; i < 4; ++i) {
		eval += sumLocation(blacks[i] + whites[i]);
	}
	if (playersInLocations(whites, blackStart)) eval += 4;
	else if (playersInLocations(blacks, whiteStart)) eval -= 4;
	return eval;
}

std::optional<std::vector<Path>> recurseFindOptimal(const Game, const Side, int, int, int, int);




#endif // AI_H
