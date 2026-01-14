#ifndef __CUTTER_H__
#define __CUTTER_H__

#include "token.h"

/**
 * TODO: decoupe la phrase en tokens
 */

// limite de tokens
#define MAX_TOKENS 100
typedef struct tokenlist
{
    token tokenTAB[MAX_TOKENS];
    int count;

} tokenlist;

/**
 * @brief initialise la liste de token
 */
tokenlist init_tokenizer(void);

/**
 * @brief decoupe la phrase en liste de tokens
 */
tokenlist tokenizer(char *phrase, tokenlist *result);

#endif