#ifndef __TOKENIZER_H__
#define __TOKENIZER_H__

#include "token.h"

/**
 * TODO: decoupe la phrase en tokens
 */

// limite de tokens
#define MAX_TOKENS 1000
typedef struct tokenlist
{
    token tokenTAB[MAX_TOKENS];
    int count = 0;

} tokenlist;

/**
 * @brief initialise la liste de token
 */
tokenlist init_tokenizer();

/**
 * @brief decoupe la phrase en liste de tokens
 */
tokenlist tokenizer(char phrase);

#endif