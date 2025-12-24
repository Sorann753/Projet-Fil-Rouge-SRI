#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "configLoader/configLoader.h"

#define LINE_MAX_LEN 512  // longueur maximale d'une ligne lue depuis le fichier

/**
 * @brief Supprime les espaces, tabulations et retours à la ligne
 *        au début et à la fin d'une chaîne
 * @param str La chaîne à nettoyer (modifiée en place)
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
        memmove(str, start, end - start + 2); // +1 pour '\0'
}

/**
 * @brief Lit la valeur d'une clé dans un fichier TOML
 *        La section [xxx] est ignorée, on cherche juste la clé globale
 * @param filename Chemin vers le fichier TOML (relatif depuis build/debug)
 * @param key Clé à chercher dans le fichier (ex: "python_simulation_path")
 * @return Chaîne mallocée contenant la valeur (à libérer par l'appelant), ou NULL si non trouvée
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

    // lire le fichier ligne par ligne
    while (fgets(line, sizeof(line), file)) {
        // ignorer les commentaires commençant par #
        char *comment_pos = strchr(line, '#');
        if (comment_pos) *comment_pos = '\0';

        trim(line);  // supprimer espaces en début/fin
        if (strlen(line) == 0) continue; // ligne vide ou commentaire

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
