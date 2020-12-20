#ifndef LESSCPP_CONST_GLOBALS_H
#define LESSCPP_CONST_GLOBALS_H

#include <array>
#include <optional>

#include <ciso646>
#include <utility>

#include "location.h"


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
