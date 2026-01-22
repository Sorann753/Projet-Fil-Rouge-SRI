/**
 * @author CHALUMEAUX Victor
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "history/history.h"
#include "configLoader/configLoader.h"

/**
 * @brief Convertit un niveau d'historique en chaîne lisible
 * @param level Niveau de l'événement
 * @return Chaîne correspondant au niveau
 */
static const char *history_level_to_string(HistoryLevel level)
{
    switch (level) {
        case INFO:
            return "INFO";
        case ERROR:
            return "ERROR";
        case WARNING:
            return "WARNING";
        default:
            return "UNKNOWN";
    }
}

static FILE *history_file = NULL;

/**
 * @brief Initialise un fichier d'historique
 *        Le nom du fichier est généré à partir de la date et de l'heure
 *        de lancement du programme
 * @return 0 si succès, -1 en cas d'erreur
 */
int history_init(void)
{
    // récupérer le chemin depuis la configuration
    char *history_path = config_loader("config/globalConfig.toml", "history_folder");
    if (!history_path) {
        fprintf(stderr, "ERROR: history_path not found in config\n");
        return -1;
    }

    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    if (!tm_info) {
        free(history_path);
        return -1;
    }

    char filename[128];
    strftime(filename, sizeof(filename),
             "history-%d-%m-%Y_%H-%M.log", tm_info);

    // construire le chemin complet : <path>/<filename>
    char fullpath[256];
    snprintf(fullpath, sizeof(fullpath),
             "../../../../%s/%s", history_path, filename);

    free(history_path);

    history_file = fopen(fullpath, "w");
    if (!history_file) {
        fprintf(stderr, "ERROR: cannot create history file %s\n", fullpath);
        return -1;
    }

    printf("Fichier historique initialisé dans %s\n", fullpath);

    // En tete du fichier .log
    fprintf(history_file, "=== Historique UPSSIBOT ===\n");
    fprintf(history_file, "Démarrage : %d/%d/%d %d:%d\n\n",
            tm_info->tm_mday,
            tm_info->tm_mon + 1,
            tm_info->tm_year + 1900,
            tm_info->tm_hour,
            tm_info->tm_min);

    fflush(history_file);
    return 0;
}


/**
 * @brief Écrit une ligne dans l'historique en cours
 * @param level Niveau de l'événement (INFO, WARNING, ERROR)
 * @param message Message descriptif à enregistrer
 */
void history_log(HistoryLevel level, const char *message)
{
    if (!history_file || !message) return;

    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    if (!tm_info) return;

    char timestamp[16];
    strftime(timestamp, sizeof(timestamp),
             "%H:%M:%S", tm_info);

    fprintf(history_file, "[%s] [%s] %s\n",
            timestamp,
            history_level_to_string(level),
            message);

    fflush(history_file);
}

/**
 * @brief Ferme le fichier d'historique en cours
 */
void history_close(void)
{
    if (!history_file) return;
    
    fprintf(history_file, "\n=== Fin de l'exécution ===\n");
    fclose(history_file);
    printf("Fichier historique fermé\n");
    history_file = NULL;
}
