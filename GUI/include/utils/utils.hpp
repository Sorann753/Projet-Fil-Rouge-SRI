#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <math.h>

struct coordinate
{
    int x;
    int y;
};

coordinate polarConvert (float randius, float angle);

#endif