#ifndef READ_LIDAR_H
#define READ_LIDAR_H

#define MAX_POINTS 1000
#define LINE_BUFFER_SIZE 256

typedef struct {
    float angle;
    float distance;
} LidarPoint;

typedef struct {
    LidarPoint points[MAX_POINTS];
    int count;
} LidarBuffer;

LidarBuffer get_lidar_scan(void);

#endif