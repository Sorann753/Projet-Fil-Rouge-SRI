#ifndef READ_LIDAR_H
#define READ_LIDAR_H

#include "utils/position.h"

#define MAX_POINTS 1000
#define LINE_BUFFER_SIZE 256

PolarCoordinate* get_lidar_scan(int *count);

#endif