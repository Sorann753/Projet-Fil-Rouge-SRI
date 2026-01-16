/**
 * @author GHOUILEM ABDELHAFIDH
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "manualPilot/vocabulary.h"
#include "configLoader/configLoader.h"

/**
 * @brief ajoute plusieurs mots avec les mêmes toks
 * @param arbre Pointeur vers le pointeur de l'arbre
 * @param toml_phrase "['avance', 'va']"
 * @param type Le type de token (TOK_VERBE, TOK_COLOR, etc.)
 * @param action L'action associée (si c'est un verbe)
 * @param color La couleur associée (si c'est une couleur)
 * @param direction La direction associée (si c'est une direction)
 */
void add_words(TreeMap **arbre, char *toml_phrase, tokentype type, action_t action, color_t color, direction_t direction)
{
    /* Vérifier que la liste n'est pas NULL */
    if (toml_phrase == NULL)
    {
        return;
    }

    /* Découper la liste aux caractères: [ ] ' " , espace */
    char *mot = strtok(toml_phrase, "[]'\", \t\n");

    /*parcours toute la phrase de synonymes (mise en parametre)*/
    while (mot != NULL)
    {
        /* créer un token pour ce mot et allouer de la memoir*/
        token *tok = malloc(sizeof(token));

        if (tok == NULL)
        {
            fprintf(stderr, "vocabulary: erreur malloc fonction add_word\n");
            return;
        }

        /* rmplir les toks du token */
        strncpy(tok->texte, mot, 100);
        tok->texte[100] = '\0';    /*pour sassurer quil y as un \0 a la fin*/
        tok->type = type;          /*mis en parametre de la fonction*/
        tok->data.action = action; /* mis en parametre de la fontion*/

        /* en fonction du type*/
        if (type == TOK_COLOR)
        {
            tok->data.color = color; /*mis en parametre*/
        }
        else if (type == TOK_VERBE)
        {
            tok->data.action = action; /*mis en parametre*/
        }
        /* rmq: l'union on ne peut utiliser qu'un seul variable à la fois */

        /* Ajouter dans l'arbre */
        insertValue(arbre, makeKey(mot), tok, true);

        /* Passer au mot suivant */
        mot = strtok(NULL, "[]'\", \t\n");
    }
}

/**
 * @brief Charge le vocabulaire depuis un fichier TOML
 * @param lemplacement du toml.fr
 */
TreeMap *vocabulary_load(const char *filepath)
{

    /* intialiser l'arbre vide */
    TreeMap *arbre = initTreeMap();
    if (arbre == NULL)
    {
        fprintf(stderr, "vocabulary: erreur initTreeMap\n");
        return NULL;
    }

    char *phrase;

    /*LES VERBES DU TOML*/

    /* Forward */

    /*lire la phrase brut du toml.fr apres forward: ...... */
    phrase = config_loader(filepath, "forward");

    add_words(&arbre, phrase, TOK_VERBE, ACT_FORWARD, COL_NONE, DIR_NONE); /*ajouter */
    free(phrase);

    /* Backward */
    phrase = config_loader(filepath, "backward");
    add_words(&arbre, phrase, TOK_VERBE, ACT_BACKWARD, COL_NONE, DIR_NONE);
    free(phrase);

    /* Turn */
    phrase = config_loader(filepath, "turn");
    add_words(&arbre, phrase, TOK_VERBE, ACT_TURN, COL_NONE, DIR_NONE);
    free(phrase);

    /* Stop */
    phrase = config_loader(filepath, "stop");
    add_words(&arbre, phrase, TOK_VERBE, ACT_STOP, COL_NONE, DIR_NONE);
    free(phrase);

    /*DIRECTION*/

    /* Left */
    phrase = config_loader(filepath, "left");
    add_words(&arbre, phrase, TOK_VERBE, ACT_TURN, COL_NONE, DIR_LEFT);
    free(phrase);

    /* Right */
    phrase = config_loader(filepath, "right");
    add_words(&arbre, phrase, TOK_VERBE, ACT_TURN, COL_NONE, DIR_RIGHT);
    free(phrase);

    /*COULEUR*/

    /* Red */
    phrase = config_loader(filepath, "red");
    add_words(&arbre, phrase, TOK_COLOR, ACT_NONE, COL_RED, DIR_NONE);
    free(phrase);

    /* Blue */
    phrase = config_loader(filepath, "blue");
    add_words(&arbre, phrase, TOK_COLOR, ACT_NONE, COL_BLUE, DIR_NONE);
    free(phrase);

    /* Green */
    phrase = config_loader(filepath, "green");
    add_words(&arbre, phrase, TOK_COLOR, ACT_NONE, COL_GREEN, DIR_NONE);
    free(phrase);

    /*SEPARATEUR*/
    phrase = config_loader(filepath, "separator");
    add_words(&arbre, phrase, TOK_UNKNOWN, ACT_NONE, COL_NONE, DIR_NONE);
    free(phrase);

    printf("vocabulary succesfuly charged !\n");
    return arbre;
}

/**
 * @brief pour chercher un mot dans larbre de vocabulaire
 */
token *vocabulary_shearch(TreeMap *vocab, const char *mot)
{
    if (vocab == NULL || mot == NULL)
    {
        return NULL;
    }

    /* vu que la fonction retourne un void* on va cast en token* */
    return (token *)getValue(vocab, mot);
}