#ifndef HISTORY_H
#define HISTORY_H

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

typedef enum HistoryLevel {
    INFO,
    ERROR,
    WARNING
} HistoryLevel;

int history_init(void);

void history_log(HistoryLevel level, const char* message);

void history_close(void);

#endif