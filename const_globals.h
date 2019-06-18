//
// Created by aviany on 5/9/19.
//

#ifndef LESSCPP_CONST_GLOBALS_H
#define LESSCPP_CONST_GLOBALS_H

#include <utility>
#include <array>
#include <zconf.h>

typedef std::pair<int, int> Location;
typedef std::array<int, 3> WallSeg;
typedef std::array<WallSeg, 3> WallConfig;


constexpr float block_size = 200;
constexpr float block_border = 2;
constexpr float player_size = 30;

constexpr uint window_width = 800;
constexpr uint window_height = 600;

#endif //LESSCPP_CONST_GLOBALS_H
