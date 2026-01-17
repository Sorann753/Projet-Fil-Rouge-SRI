#ifndef __PARSER_H__
#define __PARSER_H__

#include "manualPilot/command.h"
#include "manualPilot/cutter.h"

/**
 * @brief Initialise une liste de commandes vide
 */
command_list init_command_list(void);

/**
 * @brief transformer la liste de tokens en commandes
 */
int parser(tokenlist *token_list, command_list *cmd_list);

#endif