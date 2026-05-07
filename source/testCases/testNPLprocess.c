#include <stdio.h>
#include "manualPilot/NPLprocess.h"
#include "manualPilot/vocabulary.h"
#include "manualPilot/command.h"
#include "testCases/testNPLprocess.h"

int testNPLprocess(void)
{
    printf("=== BANC D'ESSAI NLP ===\n");

    /*charger le vocabulaire*/
    TreeMap *arbre_vocabulaire = vocabulary_load("config/vocabulary/fr.toml");

    if (arbre_vocabulaire == NULL)
    {
        printf("ERREUR : Impossible de charger le dictionnaire.\n");
        return 1;
    }

    /*phrase de test*/
    char *phrase_test = "avance de 100 puis tourne a droite et continue ensuite avance tout droite et ne tourne pas ";
    printf("Phrase testee : \"%s\"\n\n", phrase_test);

    /*utiliser npl pour avoir la liste de commande*/
    command_list ma_liste = npl(arbre_vocabulaire, phrase_test);

    /*log*/
    printf("Nombre de commandes generees : %d\n", ma_liste.count);
    printf("----------------------------------\n");

    for (int i = 0; i < ma_liste.count; i++)
    {
        printf("Commande [%d] :\n", i);
        printf("> Action    : %d\n", ma_liste.cmd[i].action);
        printf("> Valeur    : %.2f\n", ma_liste.cmd[i].value);
        printf("> Direction : %d\n", ma_liste.cmd[i].direction);
        printf("> Negation  : %d\n", ma_liste.cmd[i].negation);
        printf("----------------------------------\n");
    }

    freeTreeMap(&arbre_vocabulaire);

    return 0;
}