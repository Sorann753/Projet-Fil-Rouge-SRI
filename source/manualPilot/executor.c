#include <stdio.h>
#include "manualPilot/executor.h"
#include "history/history.h"

/**
 * TODO: execute les commandes avec les fonction de simullatorController
 */
void execut_cmd(command_list *cmd_list, RobotPosition *robot)
{
    if (cmd_list == NULL || robot == NULL)
    {
        fprintf(stderr, "executor: ERREUR execut_cmd()\n");
        history_log(WARNING, "executor: ERREUR execut_cmd()");
        return;
    }
    int i;
    for (i = 0; i < cmd_list->count; i++)
    {
        switch (cmd_list->cmd[i].action)
        {
        case ACT_FORWARD:
            forward(cmd_list->cmd[i].value, robot);
            break;

        case ACT_BACKWARD:
            backward(cmd_list->cmd[i].value, robot);
            break;

        case ACT_TURN:
            if (cmd_list->cmd[i].value > 0.001)
            {
                /* si une valeur est spécifiée par l'utiliser */
                float angle = cmd_list->cmd[i].value;
                if (cmd_list->cmd[i].direction == DIR_RIGHT)
                {
                    angle = -angle; /* droite = angle négatif */
                }
                turn(angle, robot);
            }
            else if (cmd_list->cmd[i].direction == DIR_LEFT)
            {
                turn(90, robot);
            }
            else if (cmd_list->cmd[i].direction == DIR_RIGHT)
            {
                turn(-90, robot);
            }
            else
            {
                turn(90, robot); /*par default on tourne a gauche */
            }
            break;

        case ACT_STOP:
            /* (facultatif) */
            break;

        case ACT_SEARCH:
            /* TODO: Appeler la fonction de vision pour chercher un objet */
            /* search_object(cmd->color, cmd->object); */
            break;

        case ACT_NONE:
            /* pas daction alors on fait rien */
            break;

        default:
            fprintf(stderr, "executor: ERREUR executor() UNKNOWN ACTION : %d\n", cmd_list->cmd[i].action);
            history_log(WARNING, "executor: ERREUR executor() UNKNOWN ACTION ");
            return;
        }
    }
}