#include "wall.h"

WallConfig wall::rotateWallConfig(WallConfig const config, uint const rotation) {
    WallConfig tmp;
    int orientation;
    int displacement;
    int distance;

    for (size_t i = 0; i < 3; ++i) {
        if (config[i][0] == -2) {
            tmp[i] = {-2, 0, 0};
            continue;
        }
        orientation = config[i][0];
        displacement = config[i][1];
        distance = config[i][2];

        for (size_t j = 0; j < rotation; ++j) {
            displacement = orientation * displacement;
            distance = -orientation * distance;
            orientation = -orientation;
        }
        tmp[i] = {orientation, displacement, distance};
    }
    return tmp;
}



