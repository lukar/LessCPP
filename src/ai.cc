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
					if (auto aux = recurseFindOptimal(newstate, ~player, depth - 1, alpha, beta, neweval); aux.size()!=0) {
						auto const prune = std::get<2>(aux.back()); // prune = cost of the tip of the branch 
						if (player == Player::WHITE) 
							beta = std::min(beta, prune);
						else 
							alpha = std::max(alpha, prune);
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




Path alphaBeta(const Game state, int node, int depth, int alpha, int beta) {
	Game newstate = state;
	int value, new_value;
	std::vector<Move> path, new_path;

	int piece = (node / 4);
	Direction direction = static_cast<Direction>(node % 4);
	Move move = std::make_tuple(piece, direction, 0); // move template
	
	if (newstate.movePiece(piece, direction)) {// make move and check if legal
		std::get<2>(move) = evaluation(newstate);
		if (newstate.getState() == GameState::ENDED or depth == 0) {
			// Path is being built from deep (depth==0) to shallow...
			path = std::vector<Move>{ move }; path.reserve(100);
			return path;
		}

		if (newstate.active_player() == Player::WHITE) {
			value = -1000;
			for (int i = 0; i < 16; i++) {
				new_path = alphaBeta(newstate, i, depth - 1, alpha, beta);
				new_value = std::get<2>(new_path.back());
				if (new_value > value) {
					value = new_value;
					path = new_path;
				}
				alpha = std::max(alpha, value);
				if (alpha >= beta) break;
			}
			path.emplace(path.begin(), move);
			return path;
		}
		else {
			value = 1000;
			for (int i = 0; i < 16; i++) {
				new_path = alphaBeta(newstate, i, depth - 1, alpha, beta);
				new_value = std::get<2>(new_path.back());
				if (new_value < value) {
					value = new_value;
					path = new_path;
				}
				beta = std::min(beta, value);
				if (alpha >= beta) break;
			}
			path.emplace(path.begin(), move);
			return path;
		}
	// If Illegal move
	}
	if (newstate.active_player() == Player::WHITE)
		std::get<2>(move) = -10000; // move cost
	else 
		std::get<2>(move) = 10000; // move cost

	path = std::vector<Move>{ move }; path.reserve(100);
	return path;
}


std::vector<Move> findOptimalMove(Game state,int depth) {
	Game newstate = state;
	int lowest_score = 100000;
	int new_value;
	Path best_path;
	Path new_path;
	for (int i=0; i < 16; i++) {
		new_path = alphaBeta(newstate, i, depth, -10000, 10000);
		new_value = std::get<2>(new_path.front());
		if (new_value <= lowest_score) {
			lowest_score = new_value;
			best_path = new_path;
		}
	}
	return best_path;
}

std::vector<Move> findOptimalMoveStohastic(Game state, int depth) {
	Game newstate = state;
	int lowest_score = 100000;
	int new_value;
	Path best_path;
	Path new_path;
	for (int i = 0; i < 16; i++) {
		new_path = alphaBeta(newstate, i, depth, -10000, 10000);
		new_value = std::get<2>(new_path.front());
		if (new_value <= lowest_score) {
			lowest_score = new_value;
			best_path = new_path;
		}
	}
	return best_path;
}

Path alphaBetaStohastic(const Game state, int node, int depth, int alpha, int beta) {
	Game newstate = state;
	int value, new_value;
	std::vector<Move> path, new_path;

	int piece = (node / 4);
	Direction direction; // = static_cast<Direction>(node % 4);

	if (newstate.active_player() == Player::WHITE)
		direction = randomDirectionWhite();
	else
		direction = randomDirectionBlack();

	Move move = std::make_tuple(piece, direction, 0); // move template

	if (newstate.movePiece(piece, direction)) {// make move and check if legal
		std::get<2>(move) = evaluation(newstate);
		if (newstate.getState() == GameState::ENDED or depth == 0) {
			// Path is being built from deep (depth==0) to shallow...
			path = std::vector<Move>{ move }; path.reserve(100);
			return path;
		}

		if (newstate.active_player() == Player::WHITE) {
			value = -1000;
			for (int i = 0; i < 12; i++) {
				new_path = alphaBetaStohastic(newstate, i, depth - 1, alpha, beta);
				new_value = std::get<2>(new_path.back());
				if (new_value > value) {
					value = new_value;
					path = new_path;
				}
				alpha = std::max(alpha, value);
				if (alpha >= beta) break;
			}
			path.emplace(path.begin(), move);
			return path;
		}
		else {
			value = 1000;
			for (int i = 0; i < 12; i++) {
				new_path = alphaBetaStohastic(newstate, i, depth - 1, alpha, beta);
				new_value = std::get<2>(new_path.back());
				if (new_value < value) {
					value = new_value;
					path = new_path;
				}
				beta = std::min(beta, value);
				if (alpha >= beta) break;
			}
			path.emplace(path.begin(), move);
			return path;
		}
		// If Illegal move
	}
	if (newstate.active_player() == Player::WHITE)
		std::get<2>(move) = -10000; // move cost
	else
		std::get<2>(move) = 10000; // move cost

	path = std::vector<Move>{ move }; path.reserve(100);
	return path;
}

Direction randomDirectionWhite() {
	//enum class Direction { UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3 };
	int rand_n = myrng::xorshf96() % 1000;
	if (rand_n < 200)
		return Direction::UP;
	else if (rand_n < 400)
		return Direction::LEFT;
	else if (rand_n < 700)
		return Direction::DOWN;
	else
		return Direction::RIGHT;
}

Direction randomDirectionBlack() {
	int rand_n = myrng::xorshf96() % 1000;
	if (rand_n < 200)
		return Direction::DOWN;
	else if (rand_n < 400)
		return Direction::RIGHT;
	else if (rand_n < 700)
		return Direction::UP;
	else
		return Direction::LEFT;
}

unsigned long myrng::xorshf96(void) {          //period 2^96-1
	unsigned long t;
	x ^= x << 16;
	x ^= x >> 5;
	x ^= x << 1;

	t = x;
	x = y;
	y = z;
	z = t ^ x ^ y;

	return z;
}
