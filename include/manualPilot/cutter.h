#ifndef __CUTTER_H__
#define __CUTTER_H__

#include "token.h"

/**
 * TODO: decoupe la phrase en tokens
 */

/* limite de tokens */
#define MAX_TOKENS 200
#define MAX_CHAR 600

typedef struct tokenlist
{
    token tokenTAB[MAX_TOKENS];
    int count;
} tokenlist;

/**
 * @brief initialise la liste de token
 */
tokenlist init_cutter(void);

/**
 * @brief fonction pour normaliser
 */
void normaliser(char *phrase);

/**
 * @brief decoupe la phrase en liste de tokens
 */
int cutter(const char *phrase, tokenlist *result);

/**
 * @brief affichage de la liste de tokens
 */
void print_tokenlist(tokenlist *ptr_list);

#endif