#include "wall.h"

WallConfig wall::rotateWallConfig(WallConfig const config, uint const rotation) {
    WallConfig tmp;
    int orientation;
    int displacement;
    int distance;

    for (const auto& wallseg : config) {
        orientation = wallseg[0];
        displacement = wallseg[1];
        distance = wallseg[2];

        for (size_t j = 0; j < rotation; ++j) {
            displacement = orientation * displacement;
            distance = -orientation * distance;
            orientation = -orientation;
        }
        tmp.push_back({ orientation, displacement, distance });
    }
    return tmp;
}



