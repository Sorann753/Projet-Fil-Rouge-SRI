#ifndef LIDAR_H
#define LIDAR_H
#ifdef __cplusplus
extern "C" {
#endif

#include "utils/position.h"

#define MAX_TOTAL_POINTS 100000
#define LINE_BUFFER_SIZE 256

int lidar_start();
PolarCoordinate* lidar_update_scan(size_t* point_number);
void lidar_close();

#ifdef __cplusplus
}
#endif
#endif