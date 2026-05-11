#ifndef SPEECH_H
#define SPEECH_H

/**
 * @file speech.h
 * @brief Déclaration de la fonction permettant de récupérer une commande vocale via un script Python.
 * @author Victor CHALUMEAUX
 * @remarks Dépend des modules configLoader et du script Python externe.
 */

// Commande permettant une entrée audio en texte 
char *get_speech(const char *language);

#endif