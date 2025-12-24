#include <stdio.h>
#include <stdlib.h>

#include "testCases/configLoadTest.h"
#include "configLoader/configLoader.h"

int testLoadConfig(void)
{
    printf("----- TEST LOAD CONFIG -----\n");

    char *value;

    value = config_loader("config/globalConfig.toml", "langue");
    if (value) {
        printf("langue = %s\n", value);
        free(value);
    }

    value = config_loader("config/globalConfig.toml", "history_folder");
    if (value) {
        printf("history_folder = %s\n", value);
        free(value);
    }

    value = config_loader("config/simulatorConfig.toml", "initial_x");
    if (value) {
        printf("initial_x = %s\n", value);
        free(value);
    }   

    printf("----- FIN TEST LOAD CONFIG -----\n\n");


    return 0;
}
