#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "configLoader/configLoader.h"

#define LINE_MAX_LEN 512

static void trim(char *str)
{
    char *start = str;
    while (*start == ' ' || *start == '\t' || *start == '\n' || *start == '\r')
        start++;
    char *end = start + strlen(start) - 1;
    while (end > start && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r'))
        end--;
    *(end + 1) = '\0';
    if (start != str)
        memmove(str, start, end - start + 2);
}

char *config_loader(const char *filename, const char *key)
{
    char fullpath[512];
    
    snprintf(fullpath, sizeof(fullpath), "%s", filename);

    FILE *file = fopen(fullpath, "r");
    if (!file)
    {
        fprintf(stderr, "ERROR: cannot open file %s\n", fullpath);
        return NULL;
    }

    char line[LINE_MAX_LEN];
    while (fgets(line, sizeof(line), file))
    {
        char *comment_pos = strchr(line, '#');
        if (comment_pos)
            *comment_pos = '\0';

        trim(line);
        if (strlen(line) == 0)
            continue;

        char *equal_pos = strchr(line, '=');
        if (!equal_pos)
            continue;

        *equal_pos = '\0';
        char *key_str = line;
        char *value_str = equal_pos + 1;

        trim(key_str);
        trim(value_str);

        if (strcmp(key_str, key) == 0)
        {
            char *result = malloc(strlen(value_str) + 1);
            if (!result)
            {
                fclose(file);
                return NULL;
            }
            strcpy(result, value_str);
            fclose(file);
            return result;
        }
    }

    fclose(file);
    return NULL;
}