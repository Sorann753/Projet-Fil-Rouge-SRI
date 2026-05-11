/**
 * @file history.c
 * @brief Module de gestion de l'historique des événements d'UPSSIBOT.
 *        Permet d'initialiser un fichier d'historique, d'y écrire des messages
 *        avec différents niveaux (INFO, WARNING, ERROR) et de le fermer correctement.
 * @author Victor CHALUMEAUX
 * @remarks Dépend du module configLoader pour récupérer le chemin du dossier historique.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "history/history.h"
#include "configLoader/configLoader.h"

/**
 * @brief Convertit un niveau d'historique en chaîne lisible
 * @param level Niveau de l'événement (INFO, WARNING, ERROR)
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

// Pointeur vers le fichier d'historique courant
static FILE *history_file = NULL;

/**
 * @brief Initialise un fichier d'historique
 *        Le nom du fichier est généré à partir de la date et de l'heure de lancement
 *        du programme et placé dans le dossier spécifié dans la configuration.
 * @return 0 si succès, -1 en cas d'erreur
 * @remarks Écrit un en-tête dans le fichier avec la date et l'heure de démarrage.
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

    // En-tête du fichier .log
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
 * @remarks Ajoute un timestamp et le niveau dans chaque ligne.
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
 * @remarks Écrit la date et l'heure de fin dans le fichier et ferme le flux.
 */
void history_close(void)
{
    if (!history_file) return;

    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    struct tm tm_backup;

    if (!tm_info) {
        // créer une structure locale pour ne pas pointer sur une temporaire
        tm_backup = (struct tm){0};
        tm_info = &tm_backup;
    }

    fprintf(history_file, "\nFin : %d/%d/%d %d:%d\n",
            tm_info->tm_mday,
            tm_info->tm_mon + 1,
            tm_info->tm_year + 1900,
            tm_info->tm_hour,
            tm_info->tm_min);

    fprintf(history_file, "=== Fin de l'exécution ===\n");

    fflush(history_file);
    fclose(history_file);
    printf("Fichier historique fermé\n");
    history_file = NULL;
}
