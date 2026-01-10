/**
 * @author GHOUILEM Abdelhafidh
 */

#ifndef __VOCABULARY_LOADER_H__
#define __VOCABULARY_LOADER_H__

/**
 * TODO: Charger le dico depuis fr.toml
 * TODO: Chercher un mot dans le dico
 * TODO: Librer la memoir
 */

#define MAX_SYN 30
#define MAX_LEN_WORD 20
/**
 * @brief Le dictionnaire complet (contient tous les synonymes pour chaque type d'action)
 * remarque: count cest pour compter le nombre de synonyme par mot
 */
typedef struct
{
    char forward[MAX_SYN][MAX_LEN_WORD]; /* Synonymes de "avancer" */
    int forward_count;

    char backward[MAX_SYN][MAX_LEN_WORD]; /* Synonymes de "reculer" */
    int backward_count;

    char turn[MAX_SYN][MAX_LEN_WORD]; /* Synonymes de "tourner" */
    int turn_count;

    char stop[MAX_SYN][MAX_LEN_WORD]; /* Synonymes de "stop" */
    int stop_count;

    char search[MAX_SYN][MAX_LEN_WORD]; /* Synonymes de "chercher" */
    int search_count;

    /*DIRECTIONS*/
    char left[MAX_SYN][MAX_LEN_WORD]; /* Synonymes de "gauche" */
    int left_count;

    char right[MAX_SYN][MAX_LEN_WORD]; /* Synonymes de "droite" */
    int right_count;

    /*COULEUR*/
    char red[MAX_SYN][MAX_LEN_WORD];
    int red_count;

    char blue[MAX_SYN][MAX_LEN_WORD];
    int blue_count;

    char green[MAX_SYN][MAX_LEN_WORD];
    int green_count;

    /*CONNECTEURS*/
    char connectors[MAX_SYN][MAX_LEN_WORD]; /* "et", "puis", "ensuite" */
    int connectors_count;

    /*NÉGATION*/
    char negation[MAX_SYN][MAX_LEN_WORD]; /* "ne", "pas", "jamais" */
    int negation_count;

} Vocabulary;

/*--- PROTOTYPES ---*/

/**
 * @brief lire le fichier TOML avec (remplace dans la structure vocabylary)
 */
void load_vocabulary(char file_name, Vocabulary *voc);

#endif