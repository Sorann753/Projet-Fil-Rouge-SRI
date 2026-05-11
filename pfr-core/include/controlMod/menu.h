#ifndef MENU_H
#define MENU_H

/**
 * @file menu.h
 * @brief Déclarations du module de gestion des menus d'UPSSIBOT : menu principal, contrôle,
 *        langue et simulation. Permet la navigation entre différents modes et la saisie
 *        utilisateur (CLI ou vocale).
 * @authors Victor CHALUMEAUX, Joan BELUSCA
 * @remarks Dépend des modules : history, speech, vocabulary, manualPilot et SimulatorController.
 */

/**
 * @brief Sélectionne un caractère depuis l'entrée standard
 * @return Premier caractère lu, ou '\0' si aucune entrée
 */
char selectMenu(void);

/**
 * @brief Affiche le menu principal et gère la navigation vers les sous-menus
 */
void homeMenu(void);

/**
 * @brief Menu de contrôle du robot (CLI ou vocal)
 * @remarks Gère la saisie de commandes utilisateur, leur analyse, parsing et exécution.
 */
void controlMenu(void);

/**
 * @brief Menu de sélection de la langue
 */
void languagesMenu(void);

/**
 * @brief Menu de gestion de la fenêtre de simulation
 */
void simulationMenu(void);


#endif // MENU_H
