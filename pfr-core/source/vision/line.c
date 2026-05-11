#include "vision/line.h"

inline IntCoordinate findMiddle(IntCoordinate A, IntCoordinate B){
    return (IntCoordinate){
        .x = (A.x + B.x) / 2,
        .y = (A.y + B.y) / 2,
    };
}

inline double getVecLength(Vector2d v){
    return sqrt(v.vx*v.vx + v.vy*v.vy);
}

/**
 * @brief construct a normalised vector in a random direction
 */
Vector2d makeRandomVec(){
    Vector2d vec = {
        .vx = (rand() % 2001) - 1000,
        .vy = (rand() % 2001) - 1000,
    };
    /** @post vx and vy have values in range [-1000, 1000] */

    double norm = getVecLength(vec);

    vec.vx /= norm;
    vec.vy /= norm;
    /** @post vec has as norm of 1 */

    return vec;
}

inline MathLine makeLine(IntCoordinate refPoint, Vector2d directingVec){
    return (MathLine){
        .direct = directingVec,
        .point = refPoint,
    };
}

MathLine perpendicular(MathLine base, const IntCoordinate* crossingPoint){
    if(crossingPoint == NULL){
        crossingPoint = &base.point;
    }

    Vector2d norm = {
        .vx = -base.direct.vy,
        .vy = base.direct.vx,
    };

    return (MathLine){
        .direct = norm,
        .point = *crossingPoint,
    };
}
