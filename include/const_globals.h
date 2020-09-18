//
// Created by aviany on 5/9/19.
//

#ifndef LESSCPP_CONST_GLOBALS_H
#define LESSCPP_CONST_GLOBALS_H

#include <array>
#include <optional>

#include <ciso646>

typedef unsigned int uint;

enum class Player {WHITE, BLACK, NONE};

constexpr Player operator~(const Player& player) {
    if (player == Player::WHITE) return Player::BLACK;
    else return Player::WHITE;
}

enum class State {ONGOING, LAST_TURN, ENDED};
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

struct Location {
	int x, y;
    constexpr bool operator==(const Location &rhs) const {return x == rhs.x && y == rhs.y;}
    bool operator<(const Location &rhs) const {return x*10+y < rhs.x*10 +rhs.y;}
	constexpr std::optional<Location> operator+(const Direction direction) const {
        Location tmp = *this;
        if(direction == Direction::UP) {
            if (tmp.y == 0) return {};
            tmp.y--;
        }
        else if(direction == Direction::DOWN){
            if (tmp.y == 5) return {};
            tmp.y++;
        }
        else if(direction == Direction::LEFT){
            if (tmp.x == 0) return {};
            tmp.x--;
        }
        else {  // (direction == Direction::RIGHT)
            if (tmp.x == 5) return {};
            tmp.x++;
        }
        return tmp;
    }

    constexpr Location operator+(const Location location) const {
        return {this->x + location.x, this->y + location.y};
    }

    constexpr Location operator-(const Location location) const {
        return {this->x - location.x, this->y - location.y};
    }
};

template <size_t N> using Locations = std::array<Location, N>;

typedef std::array<int, 3> WallSeg;
typedef std::array<WallSeg, 3> WallConfig;

// individual color pieces starting positions
constexpr Locations<4> whiteStart = {{{0, 0},{1, 0},{0, 1},{1, 1}}};
constexpr Locations<4> blackStart = {{{4, 4},{5, 4},{4, 5},{5, 5}}};

constexpr float block_size = 200;
constexpr float border_width = 2;
constexpr float line_width = 5;
constexpr float wall_width = 20;
constexpr float piece_size = 30;

constexpr int window_width = 800;
constexpr int window_height = 600;


#endif //LESSCPP_CONST_GLOBALS_H
