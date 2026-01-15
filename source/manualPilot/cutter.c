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
    /**
     * strtok(phrase, " ") : decouper la phrase (replace " " par \0)
     * strncpy(prase1, phrase2, nb_char_limite) : copier une phrase
     */

    // copier la phrase
    char temp[];
    strncpy(phrase, temp);

    // decouper la phrase
    strtok(temp, " ");

    // mettre les mots dans tokenlist
    int i = 0;

    for (int j; i < len(temp), j++)
    {
        while (temp[i] != "\0")
        {
            char mot[i] = temp[i]

                i++;
        }
        result->tokenTAB
            [j] = mot;
    }
}