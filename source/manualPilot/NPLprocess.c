#include "manualPilot/NPLprocess.h"
#include "manualPilot/cutter.h"
#include "manualPilot/analysis.h"
#include "manualPilot/parser.h"
#include <stdio.h>

/**
 * @brief (Cutter -> Analyze -> Parser)
 * @param vocab_tree L'arbre binaire contenant le dictionnaire (chargé au démarrage)
 * @param phrase La chaîne de caractères brute tapée par l'utilisateur
 * @return Une structure command_list prête à être envoyée ou exécutée
 */
command_list npl(TreeMap *vocab_tree, const char *phrase)
{
    // initialiser la listre de token et liste de commande
    tokenlist tok_list = init_cutter();
    command_list final_cmd_list = init_command_list();

    if (vocab_tree == NULL || phrase == NULL)
    {
        fprintf(stderr, "fullParser: ERREUR - Arbre ou phrase NULL\n");
        return final_cmd_list; // Retourne count = 0
    }

    // Decoupage
    if (cutter(phrase, &tok_list) < 0)
    {
        fprintf(stderr, "fullParser: ERREUR lors du cutter\n");
        return final_cmd_list;
    }

    // Typage
    if (analyze(vocab_tree, &tok_list) < 0)
    {
        fprintf(stderr, "fullParser: ERREUR lors de l'analysis\n");
        return final_cmd_list;
    }

    // Assemblage
    if (parser(&tok_list, &final_cmd_list) < 0)
    {
        fprintf(stderr, "fullParser: ERREUR lors du parser\n");
        return final_cmd_list;
    }

    return final_cmd_list;
}
