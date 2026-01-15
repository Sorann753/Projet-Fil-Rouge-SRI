/**
 * @author GHOUILEM ABDELHAFIDH
 */
#ifndef __VOCABULARY_H__
#define __VOCABULARY_H__

#include "utils/treeMap.h"
#include "manualPilot/token.h"

/**
 * @brief Fonction helper : ajoute plusieurs mots avec les mêmes infos
 * @param arbre Pointeur vers le pointeur de l'arbre
 * @param raw_list La chaîne toml sans decoupage "['avance', 'va']"
 * @param type (TOK_VERBE, TOK_COLOR, etc.)
 * @param action (si c'est un verbe)
 * @param color (si c'est une couleur)
 * @param direction (si c'est une direction)
 */
void add_words(TreeMap **arbre, char *raw_list, tokentype type, action_t action, color_t color, direction_t direction);

/**
 * @brief Charge le vocabulaire depuis un fichier TOML
 * @param path Chemin du fichier TOML
 * @return TreeMap* contenant tous les mots et NULL si erreur
 */
TreeMap *vocabulary_load(const char *path);

/**
 * @brief Cherche un mot dans le vocabulaire
 * @param vocab L'arbre du vocabulaire
 * @param mot Le mot à chercher
 * @return token* avec les infos du mot et NULL si erreur
 */
token *vocabulary_shearch(TreeMap *vocab, const char *mot);

#endif