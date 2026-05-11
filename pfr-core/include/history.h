#ifndef HISTORY_H
#define HISTORY_H

/**
 * @file history.h
 * @brief Déclarations du module de gestion de l'historique des événements d'UPSSIBOT.
 *        Permet d'initialiser un fichier d'historique, d'y écrire des messages
 *        avec différents niveaux (INFO, WARNING, ERROR) et de le fermer correctement.
 * @author Victor CHALUMEAUX
 * @remarks Dépend du module configLoader pour récupérer le chemin du dossier historique.
 */

/**
 * @enum HistoryLevel
 * @brief Niveaux possibles pour les messages d'historique
 */
typedef enum {
    INFO,    /**< Information générale */
    WARNING, /**< Avertissement */
    ERROR    /**< Erreur */
} HistoryLevel;

/**
 * @brief Initialise un fichier d'historique
 *        Le nom du fichier est généré à partir de la date et de l'heure de lancement
 *        du programme et placé dans le dossier spécifié dans la configuration.
 * @return 0 si succès, -1 en cas d'erreur
 * @remarks Écrit un en-tête dans le fichier avec la date et l'heure de démarrage.
 */
int history_init(void);

/**
 * @brief Écrit une ligne dans l'historique en cours
 * @param level Niveau de l'événement (INFO, WARNING, ERROR)
 * @param message Message descriptif à enregistrer
 * @remarks Ajoute un timestamp et le niveau dans chaque ligne.
 */
void history_log(HistoryLevel level, const char *message);

/**
 * @brief Ferme le fichier d'historique en cours
 * @remarks Écrit la date et l'heure de fin dans le fichier et ferme le flux.
 */
void history_close(void);

#endif // HISTORY_H
