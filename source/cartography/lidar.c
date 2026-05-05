#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "cartography/lidar.h"
#include "configLoader/configLoader.h"

PolarCoordinate lidar_data_buffer[MAX_TOTAL_POINTS];

static FILE *to_python = NULL;
static FILE *from_python = NULL;
static pid_t lidar_pid = -1;


int lidar_start() {
    if (lidar_pid != -1) return 0;

    int pipe_in[2];
    int pipe_out[2];

    if (pipe(pipe_in) != 0 || pipe(pipe_out) != 0) return -1;

    lidar_pid = fork();
    if (lidar_pid == 0) {
        close(pipe_in[1]);
        close(pipe_out[0]);
        dup2(pipe_in[0], STDIN_FILENO);
        dup2(pipe_out[1], STDOUT_FILENO);

        char *rel_path = config_loader("config/globalConfig.toml", "python_lidar_script");
        if (!rel_path) exit(1);

        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "../../../../%s", rel_path);
        free(rel_path);

        execlp("python3", "python3", "-u", full_path, NULL);
        exit(1);
    }

    close(pipe_in[0]);
    close(pipe_out[1]);
    
    to_python = fdopen(pipe_in[1], "w");
    from_python = fdopen(pipe_out[0], "r");

    if (!to_python || !from_python) return -1;

    fprintf(to_python, "START\n");
    fflush(to_python);
    
    return 0;
}

PolarCoordinate* lidar_update_scan(int* point_number) {
    if (!to_python || !from_python) return 0;

    fprintf(to_python, "SCAN\n");
    fflush(to_python);

    char line[LINE_BUFFER_SIZE];
    while (fgets(line, sizeof(line), from_python)) {
        if (strncmp(line, "END", 3) == 0) break;
        
        if (*point_number < MAX_TOTAL_POINTS) {
            if (sscanf(line, "%f,%f", &lidar_data_buffer[*point_number].theta, 
                                      &lidar_data_buffer[*point_number].dist) == 2) {
                (*point_number)++;
            }
        }
    }
    return lidar_data_buffer;
}

void lidar_close() {
    if (lidar_pid != -1) {
        if (to_python) {
            fprintf(to_python, "EXIT\n");
            fflush(to_python);
            fclose(to_python);
        }
        if (from_python) fclose(from_python);
        
        waitpid(lidar_pid, NULL, 0);
        lidar_pid = -1;
        to_python = NULL;
        from_python = NULL;
    }
}