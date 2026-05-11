/**
 * @author GHOUILEM ABDELHAFIDH
 */

#include "manualPilot/cutter.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h> /*pour strncpy et strtok */
#include "history/history.h"

/**
 * @brief initialise la liste de token
 */
tokenlist init_cutter(void)
{

    tokenlist phrase_couper;
    phrase_couper.count = 0;
    return phrase_couper;
}

/**
 * @brief fonction pour normaliser (mettre en minuscules)
 */
void normaliser(char *phrase)
{
    for (int i = 0; phrase[i] != '\0'; i++) /* ← i = 0 et '\0' */
    {
        /* si le caractère est entre A et Z (majuscule) */
        if (phrase[i] >= 'A' && phrase[i] <= 'Z')
        {
            phrase[i] += 32; /* A+32 = a */
        }
    }
}

/**
 * @brief decoupe la phrase en liste de tokens
 */
int cutter(const char *phrase, tokenlist *result)
{
    /* test des pointeurs */
    if (phrase == NULL || result == NULL)
    {
        fprintf(stderr, "cutter: ERREUR lecture phrase\n");
        history_log(WARNING, "cutter: ERREUR lecture phrase");

        return -1;
    }

    /* initialiser le compteur */
    result->count = 0;

    /* copier la phrase entiére */
    char copie[256];
    strncpy(copie, phrase, 255);
    copie[255] = '\0';

    /* decouper la copie de la phrase */
    char *mot = strtok(copie, " \t\n,.;:!?'");

    /* mettre les mots dans tokenlist */
    while (mot != NULL && result->count < MAX_TOKENS)
    {
        normaliser(mot);

        /* mettre le mot dans la liste des tokens */
        strncpy(result->tokenTAB[result->count].texte, mot, 99);
        result->tokenTAB[result->count].texte[99] = '\0';

        /* initialiser le type */
        result->tokenTAB[result->count].type = TOK_UNKNOWN;

        result->count++;

        mot = strtok(NULL, " \t\n,.;:!?'");
    }

    return result->count;
}

/**
 * @brief affichage de la liste de tokens
 */
void print_tokenlist(tokenlist *ptr_list)
{
    if (ptr_list == NULL)
    {
        fprintf(stderr, "cutter: ERROR affichage tokenlist\n");
        history_log(WARNING, "cutter: ERROR affichage tokenlist");

        return;
    }

    printf("tokenliste: [\n");
    for (int i = 0; i < ptr_list->count; i++)
    {
        printf("  %s (type: %d)\n",
               ptr_list->tokenTAB[i].texte,
               ptr_list->tokenTAB[i].type);
    }
    printf("]\n");
}
