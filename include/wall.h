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
            std::uniform_int_distribution<unsigned int> rand0to6(0, 6);
            std::uniform_int_distribution<unsigned int> rand0to3(0, 3);

            std::array<WallConfig, N> tmp;
            for (size_t i = 0; i != N; ++i) {
                tmp[i] = wall::rotateWallConfig(wall_configs[rand0to6(eng)], rand0to3(eng));
            }
            return tmp;
        }

		constexpr bool hasWallSeg(const WallConfig &config, const WallSeg &seg) {
			for (auto const& w: config) {
				if (w[0] == seg[0] and w[1] == seg[1] and w[2] == seg[2]) return true;
			}
			return false;
//			return std::find(config.begin(), config.end(), seg) != config.end();
		}


} // namespace wall

#endif /* WALL_H */
