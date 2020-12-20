#ifndef LESSCPP_DEFINITION_GLOBALS
#define LESSCPP_DEFINITION_GLOBALS

#include <utility>
#include <vector>

enum class Direction { UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3};

constexpr Direction operator++(Direction& direction) {
    switch (direction) {
        case Direction::UP:
            direction = Direction::DOWN;
            break;
        case Direction::DOWN:
            direction = Direction::LEFT;
            break;
        case Direction::LEFT:
            direction = Direction::RIGHT;
            break;
        case Direction::RIGHT:
            direction = Direction::UP;
            break;
    }
    return direction;
}


typedef unsigned int uint;

enum class Player {WHITE, BLACK, NONE};

constexpr Player operator~(const Player& player) {
    if (player == Player::WHITE) return Player::BLACK;
    else return Player::WHITE;
}

enum class GameState {ONGOING, LAST_TURN, ENDED, PREVIEW};

// piece, direction
using Move = std::pair<uint, Direction>;
using Path = std::vector<Move>;


using WallSeg = std::array<int, 3>;
using WallConfig = std::array<WallSeg, 3>;

#endif // LESSCPP_DEFINITION_GLOBALS