#include "ai.h"

// path = piece, direction, evaluation
Path recurseFindOptimal(const Game state, const Player player, int depth, int alpha, int beta, int lasteval) {
	std::vector<Path> paths;

	for (int piece = 0; piece < 4; ++piece) {
		Direction direction = Direction::UP;
		do {
			if (alpha >= beta ) goto SKIPALL;
			Game newstate = state;
			if(auto location = newstate.movePiece(piece, direction)){
				auto const neweval = evaluation(newstate);
				if (newstate.active_player() == player) {
					if (auto aux = recurseFindOptimal(newstate, player, depth, alpha, beta, lasteval); aux.size() != 0) {
						aux.emplace(aux.begin(), std::make_tuple(piece, direction, neweval));
						paths.push_back(aux);
					}
				} else if (depth > 0 and state.getState() != GameState::LAST_TURN) {
//					if (evals.size() == 0) evals.emplace_back(neweval);
//					else if (player == Player::BLACK and evals[0] < neweval) evals.emplace(evals.begin(), neweval);
//					else if (evals.back() > neweval) evals.emplace_back(neweval);
//					else continue;
					if (player == Player::BLACK and neweval > (lasteval + 1)) continue;
					else if (player == Player::WHITE and neweval < (lasteval - 1)) continue;

					if (auto aux = recurseFindOptimal(newstate, ~player, depth - 1, alpha, beta, neweval); aux.size()!=0) {
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




int alphaBeta(const Game state, int node, int depth, int alpha, int beta) {
	Game newstate = state;
	int value;
	if (newstate.movePiece((node / 4), static_cast<Direction>(node % 4))) {// make move and check if legal
		if (newstate.getState() == GameState::ENDED) { return evaluation(newstate); }
		if (depth == 0) { return evaluation(newstate); } // final depth?

		if (newstate.active_player() == Player::WHITE) {
			value = -1000;
			for (int i = 0; i < 16; i++) {
				value = std::max(value, alphaBeta(newstate, i, depth - 1, alpha, beta));
				alpha = std::max(alpha, value);
				if (alpha >= beta) break;
			}
			return value;
		}
		else {
			value = 1000;
			for (int i = 0; i < 16; i++) {
				value = std::min(value, alphaBeta(newstate, i, depth - 1, alpha, beta));
				beta = std::min(beta, value);
				if (alpha >= beta) break;
			}
			return value;
		}
	// If Illegal move
	}
	if (newstate.active_player() == Player::WHITE) return -10000; 
	else return 10000;
}
std::vector<Move> findOptimalMove(Game state,int depth) {
	std::vector<Move> path;
	Game newstate = state;
	while (newstate.active_player() == Player::BLACK) {
		int lowest_score = 100000;
		int best_move=0;
		int tmp = 0;
		for (int i=0; i < 16; i++) {
			tmp = alphaBeta(newstate, i, depth, -10000, 10000);
			if (tmp <= lowest_score) {
				lowest_score = tmp;
				best_move = i;
			}
		}
		std::cout << "Best value in depth " << depth << " is: " << lowest_score << '\n';
		newstate.movePiece(best_move / 4, static_cast<Direction>(best_move % 4));
		std::cout << "Value after move is: " << evaluation(newstate) << '\n';
		path.emplace(path.end(), std::make_tuple(best_move / 4, static_cast<Direction>(best_move % 4), 0));
		
		depth--;
	}
	return path;
}
