#include "manualPilot/cutter.h"

/**
 * @brief initialise la liste de token
 */
tokenlist init_tokenizer()
{
    tokenlist phrase_couper;
    phrase_couper.count = 0;
    return phrase_couper;
}

/**
 * @brief decoupe la phrase en liste de tokens
 */
tokenlist tokenizer(char *phrase, tokenlist *result)
{
}