/**
 * @file configLoader.c
 * @brief Module pour lire des valeurs de configuration dans des fichiers TOML.
 *        Permet de récupérer la valeur d'une clé spécifique dans un fichier de configuration.
 * @author Victor CHALUMEAUX
 * @remarks Les sections [xxx] sont ignorées, seules les clés globales sont lues.
 *          La chaîne retournée doit être libérée par l'appelant.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "configLoader/configLoader.h"

#define LINE_MAX_LEN 512  /**< Longueur maximale d'une ligne lue depuis le fichier */

/**
 * @brief Supprime les espaces, tabulations et retours à la ligne
 *        au début et à la fin d'une chaîne (modification en place)
 * @param str La chaîne à nettoyer
 */
static void trim(char *str)
{
    char *start = str;

    // avancer jusqu'au premier caractère non-espace
    while (*start == ' ' || *start == '\t' || *start == '\n' || *start == '\r')
        start++;

    // pointer sur le dernier caractère réel
    char *end = start + strlen(start) - 1;
    while (end > start && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r'))
        end--;

    // marquer la fin de la chaîne
    *(end + 1) = '\0';

    // décaler la chaîne si nécessaire
    if (start != str)
        memmove(str, start, end - start + 2); 
}

/**
 * @brief Lit la valeur d'une clé dans un fichier TOML
 * @param filename Chemin vers le fichier TOML (relatif depuis la racine du projet)
 * @param key Clé à chercher dans le fichier (ex: "python_simulation_path")
 * @return Chaîne mallocée contenant la valeur (à libérer par l'appelant), ou NULL si non trouvée ou erreur
 * @remarks Les sections [xxx] sont ignorées, seules les clés globales sont lues.
 *          Les commentaires (#) en fin de ligne sont également ignorés.
 */
char *config_loader(const char *filename, const char *key)
{
    // construire le chemin complet depuis le répertoire de build
    char fullpath[512];
    snprintf(fullpath, sizeof(fullpath), "../../../../%s", filename);

    // ouvrir le fichier en lecture
    FILE *file = fopen(fullpath, "r");
    if (!file) {
        fprintf(stderr, "ERROR: cannot open file %s\n", fullpath);
        return NULL;
    }

    char line[LINE_MAX_LEN];

    while (fgets(line, sizeof(line), file)) {

        // ignorer les commentaires commençant par #
        char *comment_pos = strchr(line, '#');
        if (comment_pos) *comment_pos = '\0';

        trim(line);  
        if (strlen(line) == 0) continue; 

        // chercher le caractère '=' pour identifier clé = valeur
        char *equal_pos = strchr(line, '=');
        if (!equal_pos) continue;   // ligne sans '=' ignorée

        // séparer clé et valeur
        *equal_pos = '\0';
        char *key_str = line;
        char *value_str = equal_pos + 1;

        trim(key_str);   // supprimer espaces autour de la clé
        trim(value_str); // supprimer espaces autour de la valeur

        // si la clé correspond à celle recherchée, renvoyer une copie de la valeur
        if (strcmp(key_str, key) == 0) {
            char *result = malloc(strlen(value_str) + 1);
            if (!result) { fclose(file); return NULL; }
            strcpy(result, value_str);
            fclose(file);
            return result;
        }
    }

    // clé non trouvée
    fclose(file);
    return NULL;
}
