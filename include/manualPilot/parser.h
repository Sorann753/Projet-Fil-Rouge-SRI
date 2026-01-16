/**
 * @author GHOUILEM ABDELHAFIDH
 */
#ifndef __PARSER_H__
#define __PARSER_H__

#include "manualPilot/command.h"
#include "manualPilot/cutter.h"

/**
 * @brief Initialise une liste de commandes vide
 */
command_list init_command_list(void);

/**
 * @brief initialise une commande vide
 */
static void init_command(command_list *cmd_list);

/**
 * @brief transformer la liste de tokens en commandes
 * @param len_command_list retourne la taille de la liste de commande
 */
int parser(tokenlist *token_list, command_list *cmd_list);

#endif