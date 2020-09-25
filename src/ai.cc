#include "ai.h"

// path = piece, direction, evaluation
std::vector<Path> recurseFindOptimal(const Game state, const Player player, int depth, int alpha, int beta, int lasteval) {
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
					paths.emplace_back(std::vector<Path>{std::make_tuple(piece, direction, neweval)});
				}
			}
		} while (++direction != Direction::UP);
	}
SKIPALL:
	if (paths.empty()) return {};
	if (player == Player::BLACK)
		return *std::min_element(paths.begin(), paths.end(), [](std::vector<Path> a, std::vector<Path> b){ return std::get<2>(a.back()) < std::get<2>(b.back());});
	else
		return *std::max_element(paths.begin(), paths.end(), [](std::vector<Path> a, std::vector<Path> b){ return std::get<2>(a.back()) < std::get<2>(b.back());});
}




int alphaBeta(const Game state, int depth, int alpha, int beta, const Player player) {
	Game newstate = state;
	int value;
	if (depth==0) return evaluation(newstate);
	if (newstate.active_player() == player) {
		value = -1000;
		for (int i = 0; i < 16; i++) {
			if (auto location = newstate.movePiece(i / 4, (Direction)(i % 4))) {
				value = alphaBeta(newstate, --depth, -1000, 1000, Player::WHITE);
			}
			else { value = -1000; }
			if (value > alpha) alpha = value;
			if (alpha >= beta) break;
			
		}
		return value;
	}
	else {
		value = 1000;
		for (int i = 0; i < 16; i++) {
			if (auto location = newstate.movePiece(i / 4, (Direction)(i % 4))) {
				value = alphaBeta(newstate, --depth, -1000, 1000, Player::WHITE);
			}
			else { value = 1000; }
			if (value < alpha) alpha = value;
			if (alpha <= beta) break;
		}
		return value;
	}
}
std::vector<Path> findOptimalMove(Game state,int depth) {
	std::vector<Path> path;
	Game newstate = state;
	for (int j=0; j < 3; j++) {
		int best_score = 1000;
		int best_move=0;
		int tmp = 0;
		Direction direction;
		for (int i=0; i < 16; i++) {
			tmp = alphaBeta(newstate, depth, -1000, 1000, Player::WHITE);
			if (tmp <= best_score) {
				best_score = tmp;
				best_move = i;
			}
		}
		newstate.movePiece(best_move / 4, (Direction)(best_move % 4));
		path.emplace(path.begin(), std::make_tuple(best_move / 4, (Direction)(best_move % 4), 0));
	}
	return path;
}
