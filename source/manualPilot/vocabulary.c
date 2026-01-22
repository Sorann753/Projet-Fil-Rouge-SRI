/**
 * @author GHOUILEM ABDELHAFIDH
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "manualPilot/vocabulary.h"
#include "configLoader/configLoader.h"
#include "history/history.h"

/**
 * @brief ajoute plusieurs mots avec les mêmes toks
 */
void add_words(TreeMap **arbre, char *toml_phrase, tokentype type, action_t action, color_t color, direction_t direction)
{
    if (toml_phrase == NULL)
    {
        return;
    }

    char *mot = strtok(toml_phrase, "[]'\", \t\n");

    while (mot != NULL)
    {
        token *tok = malloc(sizeof(token));

        if (tok == NULL)
        {
            fprintf(stderr, "vocabulary: erreur malloc fonction add_word\n");
            history_log(WARNING, "vocabulary: erreur malloc fonction add_word");
            return;
        }

        strncpy(tok->texte, mot, 99); /*99 car (de 0 a 100)*/
        tok->texte[99] = '\0';
        tok->type = type;

        /* Stocker la bonne donnée selon le type */
        if (type == TOK_COLOR)
        {
            tok->data.color = color;
        }
        else if (type == TOK_DIRECTION)
        {
            tok->data.direction = direction;
        }
        else if (type == TOK_VERBE)
        {
            tok->data.action = action;
        }

        insertValue(arbre, makeKey(mot), tok, true);
        mot = strtok(NULL, "[]'\", \t\n");
    }
}

/**
 * @brief Charge le vocabulaire depuis un fichier TOML
 */
TreeMap *vocabulary_load(const char *filepath)
{
    TreeMap *arbre = initTreeMap();
    if (arbre == NULL)
    {
        fprintf(stderr, "vocabulary: erreur initTreeMap\n");
        history_log(WARNING, "vocabulary: erreur initTreeMap");
        return NULL;
    }

    char *phrase;

    /* VERBES */
    phrase = config_loader(filepath, "forward");
    add_words(&arbre, phrase, TOK_VERBE, ACT_FORWARD, COL_NONE, DIR_NONE);
    free(phrase);

    phrase = config_loader(filepath, "backward");
    add_words(&arbre, phrase, TOK_VERBE, ACT_BACKWARD, COL_NONE, DIR_NONE);
    free(phrase);

    phrase = config_loader(filepath, "turn");
    add_words(&arbre, phrase, TOK_VERBE, ACT_TURN, COL_NONE, DIR_NONE);
    free(phrase);

    phrase = config_loader(filepath, "stop");
    add_words(&arbre, phrase, TOK_VERBE, ACT_STOP, COL_NONE, DIR_NONE);
    free(phrase);

    phrase = config_loader(filepath, "search");
    add_words(&arbre, phrase, TOK_VERBE, ACT_SEARCH, COL_NONE, DIR_NONE);
    free(phrase);

    /* DIRECTIONS*/
    phrase = config_loader(filepath, "left");
    add_words(&arbre, phrase, TOK_DIRECTION, ACT_NONE, COL_NONE, DIR_LEFT);
    free(phrase);

    phrase = config_loader(filepath, "right");
    add_words(&arbre, phrase, TOK_DIRECTION, ACT_NONE, COL_NONE, DIR_RIGHT);
    free(phrase);

    /* COULEURS */
    phrase = config_loader(filepath, "red");
    add_words(&arbre, phrase, TOK_COLOR, ACT_NONE, COL_RED, DIR_NONE);
    free(phrase);

    phrase = config_loader(filepath, "blue");
    add_words(&arbre, phrase, TOK_COLOR, ACT_NONE, COL_BLUE, DIR_NONE);
    free(phrase);

    phrase = config_loader(filepath, "green");
    add_words(&arbre, phrase, TOK_COLOR, ACT_NONE, COL_GREEN, DIR_NONE);
    free(phrase);

    /* SEPARATEUR */
    phrase = config_loader(filepath, "separator");
    add_words(&arbre, phrase, TOK_UNKNOWN, ACT_NONE, COL_NONE, DIR_NONE);
    free(phrase);

    return arbre;
}

/**
 * @brief pour chercher un mot dans larbre de vocabulaire
 */
token *vocabulary_search(TreeMap *vocab, const char *mot)
{
    if (vocab == NULL || mot == NULL)
    {
        return NULL;
    }
    return (token *)getValue(vocab, mot);
}