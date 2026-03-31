#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_lidar.h"
#include "configLoader/configLoader.h"

#define COMMAND_BUFFER_SIZE 512

LidarBuffer get_lidar_scan(void)
{
    LidarBuffer buffer;
    buffer.count = 0;
    
    char line[LINE_BUFFER_SIZE];
    char command[COMMAND_BUFFER_SIZE];
    char *python_path = NULL;
    FILE *python_file = NULL;

    python_path = config_loader("config/globalConfig.toml", "python_lidar_script");

    if (!python_path)
    {
        return buffer;
    }

    snprintf(command, sizeof(command), "python3 ../../../../%s 2>/dev/null", python_path);
    free(python_path);

    python_file = popen(command, "r");
    if (!python_file)
    {
        return buffer;
    }

    while (fgets(line, sizeof(line), python_file) != NULL && buffer.count < MAX_POINTS)
    {
        if (sscanf(line, "%f,%f", &buffer.points[buffer.count].angle, 
                                  &buffer.points[buffer.count].distance) == 2)
        {
            buffer.count++;
        }
    }

    pclose(python_file);
    return buffer;
}