#include "ai.h"


bool isLesserPath(EvalPath a, EvalPath b) {
    return a.second < b.second;
}

EvalPath recurseFindOptimal(const GameRef state, const Player player, int depth, int alpha, int beta) {
    std::vector<EvalPath> paths;

    for (int piece = 0; piece < 4; ++piece) {
        Direction direction = Direction::UP;
        do {
            if (alpha >= beta ) goto SKIPALL;
            GameRef newstate = state;
            if(auto location = newstate.movePiece(piece, direction)){
                auto const neweval = evaluation(newstate);
                if (newstate.active_player() == player) {
                    if (auto && [path, eval] = recurseFindOptimal(newstate, player, depth, alpha, beta); path.size() != 0) {
                        path.emplace(path.begin(), std::make_pair(piece, direction));
                        paths.emplace_back(std::make_pair(path, eval));
                    }
                } else if (depth > 0 and state.getState() != GameState::LAST_TURN) {
                    if (auto && [path, prune] = recurseFindOptimal(newstate, ~player, depth - 1, alpha, beta); path.size() != 0) {
                        if (player == Player::WHITE) beta = std::min(beta, prune);
                        else alpha = std::max(alpha, prune);
                        path.emplace(path.begin(), std::make_pair(piece, direction));
                        paths.emplace_back(std::make_pair(path, prune));
                    }
                } else {
                    const auto move = std::make_pair(piece, direction);
                    const std::vector<Move> moves = {move};
                    const auto path = std::make_pair(moves, neweval);
                    paths.emplace_back(path);
                }
            }
        } while (++direction != Direction::UP);
    }
SKIPALL:
    if (paths.empty()) return {};
    if (player == Player::BLACK)
        return *std::min_element(paths.begin(), paths.end(), isLesserPath);
    else
        return *std::max_element(paths.begin(), paths.end(), isLesserPath);
}

int piecesApart(Locations<4> pieces) {
    int result = 0;
    for (const auto & a : pieces) {
        for (const auto & b : pieces) {
            if ( a == b ) continue;
            result += locationDistance(a, b);
        }
    }
    return result - 16;
}

int sumLocation(const Location& loc) {
    return loc.x + loc.y;
}

int locationDistance(const Location& a, const Location& b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

int evaluation(const GameRef & game) {
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