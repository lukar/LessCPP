#include "ai.h"

// path = player, direction, evaluation
std::optional<std::vector<Path>> recurseFindOptimal(const Game state, const Side side, int depth, int alpha, int beta, int lasteval) {
	std::vector<std::vector<Path>> paths;
//	std::vector<int> evals;

	for (int player = 0; player < 4; ++player) {
		Direction direction = Direction::UP;
		do {
			if (alpha >= beta ) goto SKIPALL;
			Game newstate = state;
			if(auto location = newstate.movePlayer(player, direction)){
				auto const neweval = evaluation(newstate);
				if (newstate.active_side() == side) {
					if (auto aux = recurseFindOptimal(newstate, side, depth, alpha, beta, lasteval)) {
						aux.value().emplace(aux.value().begin(), std::make_tuple(player, direction, neweval));
						paths.push_back(aux.value());
					}
				} else if (depth > 0 and state.getState() != State::LAST_TURN) {
//					if (evals.size() == 0) evals.emplace_back(neweval);
//					else if (side == Side::BLACK and evals[0] < neweval) evals.emplace(evals.begin(), neweval);
//					else if (evals.back() > neweval) evals.emplace_back(neweval);
//					else continue;
					if (side == Side::BLACK and neweval > lasteval - 2) continue;
					else if (side == Side::WHITE and neweval < lasteval + 2) continue;

					if (auto aux = recurseFindOptimal(newstate, ~side, depth - 1, alpha, beta, neweval)) {
						auto const prune = std::get<2>(aux.value().back());
						if (side == Side::WHITE) beta = std::min(beta, prune);
						else alpha = std::max(alpha, prune);
						aux.value().emplace(aux.value().begin(), std::make_tuple(player, direction, neweval));
						paths.push_back(aux.value());
					}
				} else {
					paths.emplace_back(std::vector<Path>{std::make_tuple(player, direction, neweval)});
				}
			}
		} while (++direction != Direction::UP);
	}
SKIPALL:
	if (paths.empty()) return std::nullopt;
	if (side == Side::BLACK)
		return *std::min_element(paths.begin(), paths.end(), [](std::vector<Path> a, std::vector<Path> b){ return std::get<2>(a.back()) < std::get<2>(b.back());});
	else
		return *std::max_element(paths.begin(), paths.end(), [](std::vector<Path> a, std::vector<Path> b){ return std::get<2>(a.back()) < std::get<2>(b.back());});
}
