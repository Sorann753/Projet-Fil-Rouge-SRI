#ifndef CONFIGLOADER_H
#define CONFIGLOADER_H

/**
 * @file configLoader.h
 * @brief Déclarations du module de lecture de fichiers de configuration TOML.
 *        Permet de récupérer la valeur d'une clé spécifique dans un fichier de configuration.
 * @author Victor CHALUMEAUX
 * @remarks Les sections [xxx] dans le fichier TOML sont ignorées.
 *          La chaîne retournée doit être libérée par l'appelant.
 */

/**
 * @brief Lit la valeur d'une clé dans un fichier TOML
 * @param filename Chemin vers le fichier TOML (relatif depuis la racine du projet)
 * @param key Clé à chercher dans le fichier (ex: "python_simulation_path")
 * @return Chaîne mallocée contenant la valeur correspondante à la clé
 *         ou NULL si la clé n'est pas trouvée ou si une erreur se produit.
 * @remarks La clé doit être globale (pas dans une section), et les commentaires (#) en fin de ligne sont ignorés.
 */
char *config_loader(const char *filename, const char *key);

#endif // CONFIGLOADER_H
