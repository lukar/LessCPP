#include "ai.h"

// path = piece, direction, evaluation
std::optional<std::vector<Path>> recurseFindOptimal(const Game state, const Player player, int depth, int alpha, int beta, int lasteval) {
	std::vector<std::vector<Path>> paths;
//	std::vector<int> evals;

	for (int piece = 0; piece < 4; ++piece) {
		Direction direction = Direction::UP;
		do {
			if (alpha >= beta ) goto SKIPALL;
			Game newstate = state;
			if(auto location = newstate.movePiece(piece, direction)){
				auto const neweval = evaluation(newstate);
				if (newstate.active_player() == player) {
					if (auto aux = recurseFindOptimal(newstate, player, depth, alpha, beta, lasteval)) {
						aux.value().emplace(aux.value().begin(), std::make_tuple(piece, direction, neweval));
						paths.push_back(aux.value());
					}
				} else if (depth > 0 and state.getState() != GameState::LAST_TURN) {
//					if (evals.size() == 0) evals.emplace_back(neweval);
//					else if (player == Player::BLACK and evals[0] < neweval) evals.emplace(evals.begin(), neweval);
//					else if (evals.back() > neweval) evals.emplace_back(neweval);
//					else continue;
					if (player == Player::BLACK and neweval > lasteval - 2) continue;
					else if (player == Player::WHITE and neweval < lasteval + 2) continue;

					if (auto aux = recurseFindOptimal(newstate, ~player, depth - 1, alpha, beta, neweval)) {
						auto const prune = std::get<2>(aux.value().back());
						if (player == Player::WHITE) beta = std::min(beta, prune);
						else alpha = std::max(alpha, prune);
						aux.value().emplace(aux.value().begin(), std::make_tuple(piece, direction, neweval));
						paths.push_back(aux.value());
					}
				} else {
					paths.emplace_back(std::vector<Path>{std::make_tuple(piece, direction, neweval)});
				}
			}
		} while (++direction != Direction::UP);
	}
SKIPALL:
	if (paths.empty()) return std::nullopt;
	if (player == Player::BLACK)
		return *std::min_element(paths.begin(), paths.end(), [](std::vector<Path> a, std::vector<Path> b){ return std::get<2>(a.back()) < std::get<2>(b.back());});
	else
		return *std::max_element(paths.begin(), paths.end(), [](std::vector<Path> a, std::vector<Path> b){ return std::get<2>(a.back()) < std::get<2>(b.back());});
}
