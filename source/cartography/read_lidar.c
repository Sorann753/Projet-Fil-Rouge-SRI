#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cartography/read_lidar.h"
#include "configLoader/configLoader.h"
#include "utils/position.h"

#define COMMAND_BUFFER_SIZE 512

PolarCoordinate* get_lidar_scan(int *count)
{
    *count = 0;
    char line[LINE_BUFFER_SIZE];
    char command[COMMAND_BUFFER_SIZE];
    char *python_path = NULL;
    FILE *python_file = NULL;

    PolarCoordinate *buffer = malloc(sizeof(PolarCoordinate) * MAX_POINTS);
    if (!buffer)
    {
        return NULL;
    }

    python_path = config_loader("config/globalConfig.toml", "python_lidar_script");
    if (!python_path)
    {
        free(buffer);
        return NULL;
    }

    snprintf(command, sizeof(command), "python3 ../../../../%s 2>/dev/null", python_path);
    free(python_path);

    python_file = popen(command, "r");
    if (!python_file)
    {
        free(buffer);
        return NULL;
    }

    while (fgets(line, sizeof(line), python_file) != NULL && *count < MAX_POINTS)
    {
        if (sscanf(line, "%f,%f", &buffer[*count].theta, 
                                  &buffer[*count].dist) == 2)
        {
            (*count)++;
        }
    }

    pclose(python_file);
    return buffer;
}