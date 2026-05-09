#include "utils/utils.hpp"
/*
Convert polar coordinates into cartesian
@tparam angle  in radian
*/
coordinate polarConvert (float radius, float angle){
    
    coordinate c;

    c.x = static_cast<int>(std::round(radius * std::cos(angle)));
    c.y = static_cast<int>(std::round(radius * std::sin(angle)));

    return c;
}