/**
 * @file speech.c
 * @brief Ce module fournit l'interface pour récupérer des commandes vocales via un script Python.
 *        Il lit la configuration pour déterminer le script Python à utiliser selon la langue,
 *        exécute le script et récupère la première ligne de sortie correspondant à la commande vocale.
 * @author Victor CHALUMEAUX
 * @remarks Dépend des modules configLoader et du script Python externe.
 */

/**
 * @brief Récupère une commande vocale en fonction de la langue spécifiée.
 *        Exécute le script Python correspondant et retourne la première ligne lue.
 * @param language Chaîne de caractères ("fr" ou "en") déterminant la langue du script Python
 * @return Pointeur vers une chaîne de caractères contenant la commande vocale
 *         ou un message d'erreur ("ERROR_CONFIG", "ERROR_POPEN", "err ")
 * @remarks La chaîne retournée est statique, elle ne doit pas être libérée.
 */

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

    // Choix du chemin Python et du script selon la langue
    if (strcmp(language, "en") == 0)
    {
        python_path = config_loader("config/globalConfig.toml", "python_vocal_interpreter_en");
    }
    else // Par défaut, on utilise le français
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
