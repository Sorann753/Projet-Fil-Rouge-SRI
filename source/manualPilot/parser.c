/**
 * @author GHOUILEM ABDELHAFIDH
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "manualPilot/parser.h"
#include "history/history.h"

#define DEFAULT_DISTANCE 50.0
#define DEFAULT_DIRECTION DIR_RIGHT
/**
 * @brief Initialise une liste de commandes vide
 */
command_list init_command_list(void)
{
    command_list list;
    list.count = 0;
    return list;
}

/**
 * @brief initialise une commande vide
 */
static void init_command(command_list *cmd_list)
{
    cmd_list->cmd[cmd_list->count].action = ACT_NONE;
    cmd_list->cmd[cmd_list->count].value = 0.0;
    cmd_list->cmd[cmd_list->count].color = COL_NONE;
    cmd_list->cmd[cmd_list->count].object = OBJ_NONE;
    cmd_list->cmd[cmd_list->count].direction = DIR_NONE;
    cmd_list->cmd[cmd_list->count].negation = 0;
}

/**
 * @brief transformer la liste de tokens en cmds[cmd1=action,value,color,direction, cmd2=action....]
 */
int parser(tokenlist *token_list, command_list *cmd_list)
{
    if (token_list == NULL || cmd_list == NULL)
    {
        fprintf(stderr, "parser: ERREUR pointeur null\n");
        history_log(WARNING, "parser: ERREUR pointeur null");
        return -1;
    }

    /*initialiser le compteur*/
    cmd_list->count = 0;

    int i = 0;
    while (i < token_list->count && cmd_list->count < MAX_COMMAND)
    {

        /*ajout du demi tour*/
        if (i + 1 < token_list->count && strcmp(token_list->tokenTAB[i].texte, "demi") == 0 && strcmp(token_list->tokenTAB[i + 1].texte, "tour") == 0)
        {
            init_command(cmd_list);
            cmd_list->cmd[cmd_list->count].action = ACT_TURN;
            cmd_list->cmd[cmd_list->count].value = 180.0;
            cmd_list->count++;

            i += 2; /*on saute les deux tokens "demi" et "tour"*/
            continue;
        }

        /*si le token actuelle cest un verbe*/
        if (token_list->tokenTAB[i].type == TOK_VERBE)
        {
            /* initialiser la commande */
            init_command(cmd_list);

            /* récupération de l'action */
            cmd_list->cmd[cmd_list->count].action = token_list->tokenTAB[i].data.action;

            /*parcourir toute la liste de token suivant jusquau prochain verbe*/
            int j = i + 1;

            /*initialisation de la negation a 0*/
            int negation = 0;
            if (i > 0 && (strcmp(token_list->tokenTAB[i - 1].texte, "ne") == 0 || strcmp(token_list->tokenTAB[i - 1].texte, "n") == 0))
            {
                negation = 1;
            }

            /*mettre la negation a 1 ou 0*/
            cmd_list->cmd[cmd_list->count].negation = negation;
            while (j < token_list->count && token_list->tokenTAB[j].type != TOK_VERBE)
            {

                /* si c'est un nombre */
                if (token_list->tokenTAB[j].type == TOK_NUM)
                {
                    /*remplacer la valeur de la commande par la valeur du token*/
                    cmd_list->cmd[cmd_list->count].value = token_list->tokenTAB[j].data.value;
                }

                /* si c'est une couleur */
                if (token_list->tokenTAB[j].type == TOK_COLOR)
                {
                    cmd_list->cmd[cmd_list->count].color = token_list->tokenTAB[j].data.color;
                }

                /* Si c'est une direction*/
                if (token_list->tokenTAB[j].type == TOK_DIRECTION)
                {
                    cmd_list->cmd[cmd_list->count].direction = token_list->tokenTAB[j].data.direction;
                }

                /*negation si on as les mot "ne" avant ou "pas" ou "plus" apres*/
                if (strcmp(token_list->tokenTAB[j].texte, "pas") == 0 || strcmp(token_list->tokenTAB[j].texte, "plus") == 0) /*sassurer que il y as un i-1*/
                {
                    negation = 1;
                }

                j++;
            }

            /*verifier la negation*/
            if (negation)
            {
                printf("commande[ %d ] (negation) !\n", cmd_list->count + 1);
                i = j;
                continue; /*on va sauter la commande pour eviter davoir une valeur par default*/
            }

            /* si le verbe est FORWARD ou BACKWARD avec une direction mais sans distance on le change en turn*/
            if ((cmd_list->cmd[cmd_list->count].action == ACT_FORWARD || cmd_list->cmd[cmd_list->count].action == ACT_BACKWARD) && cmd_list->cmd[cmd_list->count].direction != DIR_NONE)
            {
                cmd_list->cmd[cmd_list->count].action = ACT_TURN; /*exemple; avance a droite*/
                cmd_list->cmd[cmd_list->count].value = 0.0;
            }

            /*ajout des valeur par defeault*/

            /* si FORWARD ou BACKWARD sans valeur*/
            if ((cmd_list->cmd[cmd_list->count].action == ACT_FORWARD || cmd_list->cmd[cmd_list->count].action == ACT_BACKWARD) && cmd_list->cmd[cmd_list->count].value < 0.001) /*0.001 pour eviter lerreur float ==0 */
            {
                cmd_list->cmd[cmd_list->count].value = DEFAULT_DISTANCE;
            }

            /* Si TURN sans direction on met la direction par default*/
            if (cmd_list->cmd[cmd_list->count].action == ACT_TURN && cmd_list->cmd[cmd_list->count].direction == DIR_NONE && cmd_list->cmd[cmd_list->count].value < 0.001)
            {
                cmd_list->cmd[cmd_list->count].direction = DEFAULT_DIRECTION;
            }

            cmd_list->count++;
            i = j;
        }
        /*le cas ou on met un NUM sans verbe avant*/
        else if (token_list->tokenTAB[i].type == TOK_NUM)
        {
            init_command(cmd_list);

            /*on va mettre forward par default*/
            cmd_list->cmd[cmd_list->count].action = ACT_FORWARD;
            cmd_list->cmd[cmd_list->count].value = token_list->tokenTAB[i].data.value;

            cmd_list->count++;
            i++;
        }
        else
        {
            i++;
        }
    }

    /*retourner le nombre de commande dans la liste de commande*/
    return cmd_list->count;
}