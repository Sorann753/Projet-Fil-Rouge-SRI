#ifndef LIDAR_H
#define LIDAR_H

#include "utils/position.h"

#define MAX_TOTAL_POINTS 100000
#define LINE_BUFFER_SIZE 256

extern PolarCoordinate lidar_data_buffer[MAX_TOTAL_POINTS];
extern int current_total;

int lidar_start();
void lidar_update_scan();
void lidar_reset_buffer();
void lidar_close();

#endif