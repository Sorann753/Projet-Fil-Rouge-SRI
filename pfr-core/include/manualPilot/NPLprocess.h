#ifndef __NPL_PROCESS_H__
#define __NPL_PROCESS_H__

#include "utils/treeMap.h"
#include "manualPilot/command.h"

/**
 * @brief (Cutter -> Analyze -> Parser)
 * @param vocab_tree L'arbre binaire contenant le dictionnaire (chargé au démarrage)
 * @param phrase La chaîne de caractères brute tapée par l'utilisateur
 * @return Une structure command_list prête à être envoyée ou exécutée
 */
command_list npl(TreeMap *vocab_tree, const char *phrase);

#endif