#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "manualPilot/analysis.h"
#include "manualPilot/vocabulary.h"
#include "history/history.h"

/**
 * @brief pour savoir si le texte est un nombre
 * @return 1 si c'est un nombre et 0 sinon
 */
int is_number(char *mot)
{
    /* Vérifier que le mot n'est pas vide */
    if (mot == NULL || mot[0] == '\0')
    {
        return 0;
    }

    int i = 0;

    /* Gérer le signe négatif (optionnel) */
    if (mot[0] == '-' || mot[0] == '+')
    {
        i = 1;
        /* Si c'est juste "-" ou "+" ce n'est pas un nombre */
        if (mot[1] == '\0')
        {
            return 0;
        }
    }

    int has_dot = 0; /* Pour gérer les nombres décimaux comme "3.14" */

    /* Parcourir chaque caractère */
    for (; mot[i] != '\0'; i++)
    {
        if (mot[i] == '.')
        {
            /* Un seul point autorisé */
            if (has_dot)
            {
                return 0; /* Deux points cest pas un nombre */
            }
            has_dot = 1;
        }
        else if (mot[i] < '0' || mot[i] > '9')
        {
            /* Ce n'est pas un chiffre */
            return 0;
        }
    }

    return 1; /* C'est un nombre ! */
}

/**
 * @brief identifie le type de chaque token de la liste
 */
int analyze(TreeMap *arbre, tokenlist *tok_list)
{
    /* Vérifier les paramètres */
    if (arbre == NULL || tok_list == NULL)
    {
        fprintf(stderr, "analysis: erreur paramètres NULL\n");
        history_log(WARNING, "analysis: erreur paramètres NULL");
        return -1;
    }

    /* Parcourir chaque token de la liste de tokens */
    for (int i = 0; i < tok_list->count; i++)
    {
        /* chercher le mot dans le vocabulaire */
        token *found = vocabulary_search(arbre, tok_list->tokenTAB[i].texte);

        if (found != NULL)
        {
            /* le mot a été trouver dans le vocabulaire*/
            tok_list->tokenTAB[i].type = found->type;
            tok_list->tokenTAB[i].data = found->data;
        }
        else if (is_number(tok_list->tokenTAB[i].texte))
        {
            /*si cest un nombre */
            tok_list->tokenTAB[i].type = TOK_NUM;
            tok_list->tokenTAB[i].data.value = atof(tok_list->tokenTAB[i].texte); /*atof(): pour convertir la chaine en valeur float*/
        }
    }

    return 0;
}

/**
 * @brief affiche la liste de tokens avec leurs types
 */
void print_analysis(tokenlist *tok_list)
{
    if (tok_list == NULL)
    {
        fprintf(stderr, "analysis: ERREUR print NULL\n");
        history_log(WARNING, "analysis: ERREUR print NULL");
        return;
    }

    for (int i = 0; i < tok_list->count; i++)
    {
        printf("  [%d] \"%s\" => type:%d", i, tok_list->tokenTAB[i].texte, tok_list->tokenTAB[i].type);

        /* afficher en fonction du type du token */
        switch (tok_list->tokenTAB[i].type)
        {
        case TOK_VERBE:
            printf(" (VERBE, action:%d)\n", tok_list->tokenTAB[i].data.action);
            break;
        case TOK_NUM:
            printf(" (NUM, value:%.2f)\n", tok_list->tokenTAB[i].data.value);
            break;
        case TOK_COLOR:
            printf(" (COLOR, color:%d)\n", tok_list->tokenTAB[i].data.color);
            break;
        case TOK_DIRECTION:
            printf(" (DIRECTION, dir:%d)\n", tok_list->tokenTAB[i].data.direction);
            break;
        default:
            printf(" (UNKNOWN)\n");
            break;
        }
    }
}