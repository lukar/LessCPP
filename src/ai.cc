#include "ai.h"

// path = piece, direction, evaluation
Path recurseFindOptimal(const Game state, const Player player, int depth, int alpha, int beta) {
	std::vector<Path> paths;

	for (int piece = 0; piece < 4; ++piece) {
		Direction direction = Direction::UP;
		do {
			if (alpha >= beta ) goto SKIPALL;
			Game newstate = state;
			if(auto location = newstate.movePiece(piece, direction)){
				auto const neweval = evaluation(newstate);
				if (newstate.active_player() == player) {
					if (auto aux = recurseFindOptimal(newstate, player, depth, alpha, beta); aux.size() != 0) {
						aux.emplace(aux.begin(), std::make_tuple(piece, direction, neweval));
						paths.push_back(aux);
					}
				} else if (depth > 0 and state.getState() != GameState::LAST_TURN) {
					if (auto aux = recurseFindOptimal(newstate, ~player, depth - 1, alpha, beta); aux.size()!=0) {
						auto const prune = std::get<2>(aux.back()); // prune = cost of the tip of the branch 
						if (player == Player::WHITE) beta = std::min(beta, prune);
						else alpha = std::max(alpha, prune);
						aux.emplace(aux.begin(), std::make_tuple(piece, direction, neweval));
						paths.push_back(aux);
					}
				} else {
					paths.emplace_back(std::vector<Move>{std::make_tuple(piece, direction, neweval)});
				}
			}
		} while (++direction != Direction::UP);
	}
SKIPALL:
	if (paths.empty()) return {};
	if (player == Player::BLACK)
		return *std::min_element(paths.begin(), paths.end(), [](Path a, Path b){ return std::get<2>(a.back()) < std::get<2>(b.back());});
	else
		return *std::max_element(paths.begin(), paths.end(), [](Path a, Path b){ return std::get<2>(a.back()) < std::get<2>(b.back());});
}
