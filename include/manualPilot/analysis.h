/**
 * @author GHOUILEM ABDELHAFIDH
 */
#ifndef __ANALYSIS_H__
#define __ANALYSIS_H__

#include "manualPilot/cutter.h"
#include "utils/treeMap.h"
#include "manualPilot/vocabulary.h"

/**
 * @brief pour identifier le type et action de chaque token
 * @return 0 si OK, -1 si erreur
 */
int analyze(TreeMap *arbre, tokenlist *tok_list);

/**
 * @brief pour savoir si le texte d'un token est un nombre
 * @return 1 si nombre, 0 sinon
 */
int is_number(char *mot);

/**
 * @brief affiche la liste de tokens avec leurs types
 */
void print_analysis(tokenlist *tok_list);

#endif