#ifndef WALL_H
#define WALL_H

#include <array>
#include <random>
#include <algorithm>
#include "const_globals.h"

namespace wall {

		constexpr WallConfig bFullSide = {
						{
										{1, -1, -1},
										{1, -1, 1},
										{-2, 0, 0}
						}
		};
		constexpr WallConfig bHalfSide = {
						{
										{1, -1, 1},
										{-2, 0, 0},
										{-2, 0, 0}

						}

		};
		constexpr WallConfig bZigZag = {
						{
										{1, -1, -1},
										{-1, 0, -1},
										{1, 0, 1}
						}
		};
		constexpr WallConfig bT_Block = {
						{
										{1, -1, -1},
										{1, -1, 1},
										{-1, 0, -1}
						}
		};
		constexpr WallConfig bTopLeft = {
						{
										{1, -1, -1},
										{-1, 0, -1},
										{-2, 0, 0}

						}
		};
		constexpr WallConfig bTopRight = {
						{
										{1, 0, -1},
										{-1, 0, 1},
										{-2, 0, 0}
						}
		};
		constexpr WallConfig bBottomRight = {
						{
										{1, 0, 1},
										{-1, 1, 1},
										{-2, 0, 0}
						}
		};


		constexpr std::array<WallConfig, 7> wall_configs{
						bFullSide, bHalfSide, bZigZag, bT_Block, bTopLeft, bTopRight, bBottomRight};

        WallConfig rotateWallConfig(WallConfig wall, uint rotation);

        template<size_t N>
        std::array<WallConfig, N> generateNwallconfigs() {
            auto eng = std::default_random_engine(std::random_device()());
            std::uniform_int_distribution<uint8_t> rand0to6(0, 6);
            std::uniform_int_distribution<uint8_t> rand0to3(0, 3);

            std::array<WallConfig, N> tmp;
            for (size_t i = 0; i != N; ++i) {
                tmp[i] = wall::rotateWallConfig(wall_configs[rand0to6(eng)], rand0to3(eng));
            }
            return tmp;
        }

        bool hasWallSeg(const WallConfig &config, const WallSeg &seg);


} // namespace wall

#endif /* WALL_H */
