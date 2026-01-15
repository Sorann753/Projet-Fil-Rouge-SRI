#include <stdio.h>
#include "manualPilot/vocabulary.h"

void test_vocabulary(void)
{
    printf("----- TEST VOCABULARY -----\n\n");

    /* Charger le vocabulaire */
    TreeMap *vocab = vocabulary_load("config/vocabulary/fr.toml");

    if (vocab == NULL)
    {
        printf("ERREUR: impossible de charger le vocabulaire\n");
        return;
    }

    /*chhercher le mot "avance" */
    token *result = vocabulary_shearch(vocab, "avance");
    if (result != NULL)
    {
        printf("(trouver!) type:%d, action:%d\n", result->type, result->data.action);
    }
    else
    {
        printf("(non trouver)\n");
    }

    /*cherche le mot "rouge" */
    result = vocabulary_shearch(vocab, "rouge");
    if (result != NULL)
    {
        printf("(trouver!) type:%d, action:%d\n", result->type, result->data.action);
    }
    else
    {
        printf("(non trouver)\n");
    }

    /* Libérer la mémoire */
    freeTreeMap(&vocab);
}