/**
 * @author GHOUILEM ABDELHAFIDH
 */
#include <stdio.h>
#include "manualPilot/vocabulary.h"

/**
 * @return TYPE   => 0:TOK_VERB, 1:TOK_NUM, 2:TOK_COLOR ...etc
 * @return ACTION => 0:ACT_FORWARD, 1:ACT_BACKWARD, 2:ACT_TURN ...etc
 */

void test_vocabulary(void)
{
    printf("----- TEST VOCABULARY -----\n\n");

    printf("_______________________________________________________________________________________\n");
    printf("TYPE   => 0:TOK_VERB, 1:TOK_NUM, 2:TOK_COLOR, 3:TOK_OBJECT, 4:TOK_UNKNOWN, TOK_NONE\nACTION => 0:ACT_FORWARD, 1:ACT_BACKWARD, 2:ACT_TURN, 3:ACT_STOP, 4:ACT_SEARCH, 5:ACT_NONE\n");
    printf("_______________________________________________________________________________________\n\n");

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