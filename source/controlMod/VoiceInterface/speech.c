#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "controlMod/voiceInterface/speech.h"
#include "configLoader/configLoader.h"

#define LINE_BUFFER_SIZE 1024
#define COMMAND_BUFFER_SIZE 512

char *get_speech(const char *language)
{
    static char line[LINE_BUFFER_SIZE];
    char *python_path = NULL;
    FILE *python_file = NULL;
    char command[COMMAND_BUFFER_SIZE];
    const char *script_name;

    // Choix du chemin Python et du script selon la langue
    if (strcmp(language, "en") == 0)
    {
        python_path = config_loader("config/globalConfig.toml", "python_vocal_interpreter_en");
    }
    else
    {
        python_path = config_loader("config/globalConfig.toml", "python_vocal_interpreter_fr");
    }

    if (!python_path)
    {
        return (char *)"ERROR_CONFIG"; // Impossible de lire le chemin
    }

    // Construction sécurisée de la commande
    snprintf(command, sizeof(command), "python3 ../../../../%s 2>/dev/null", python_path);

    // Exécute le script Python
    python_file = popen(command, "r");

    free(python_path);

    if (!python_file)
    {
        return (char *)"ERROR_POPEN";
    }

    // Lire la première ligne du script Python
    if (fgets(line, sizeof(line), python_file) == NULL)
    {
        strncpy(line, "err ", sizeof(line));
        line[sizeof(line) - 1] = '\0';
    }
    else
    {
        // Supprimer le retour à la ligne éventuel
        size_t len = strlen(line);
        if (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r'))
        {
            line[len - 1] = '\0';
        }
    }

    pclose(python_file);
    return line;
}
