#ifndef AI_H
#define AI_H

#include "game.h"

#include <vector>
#include <utility>
#include <optional>


// tuple (moves, evaluation)
using EvalPath = std::pair<Path, int>;


int sumLocation(const Location& loc);

int locationDistance(const Location& a, const Location& b);

int piecesApart(Locations<4> pieces);

int evaluation(const GameRef & game);

EvalPath recurseFindOptimal(const GameRef, const Player, int, int, int);




#endif // AI_H
