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
        history_log(WARNING, "executor: ERREUR execut_cmd() - cmd_list or robot is NULL");
        return;
    }

    int i;
    for (i = 0; i < cmd_list->count; i++)
    {
        char log_buffer[128];
        snprintf(log_buffer, sizeof(log_buffer),
                 "Executing cmd[%d]: action=%d, value=%.2f, color=%d, direction=%d",
                 i, cmd_list->cmd[i].action, cmd_list->cmd[i].value,
                 cmd_list->cmd[i].color, cmd_list->cmd[i].direction);
        history_log(INFO, log_buffer);

        switch (cmd_list->cmd[i].action)
        {
        case ACT_FORWARD:
            forward(cmd_list->cmd[i].value, robot);
            history_log(INFO, "Action: ACT_FORWARD executed");
            break;

        case ACT_BACKWARD:
            backward(cmd_list->cmd[i].value, robot);
            history_log(INFO, "Action: ACT_BACKWARD executed");
            break;

        case ACT_TURN:
            if (cmd_list->cmd[i].value > 0.001)
            {
                float angle = cmd_list->cmd[i].value;
                if (cmd_list->cmd[i].direction == DIR_RIGHT)
                {
                    angle = -angle;
                }
                turn(angle, robot);
                history_log(INFO, "Action: ACT_TURN executed with specific angle");
            }
            else if (cmd_list->cmd[i].direction == DIR_LEFT)
            {
                turn(90, robot);
                history_log(INFO, "Action: ACT_TURN executed to left (90°)");
            }
            else if (cmd_list->cmd[i].direction == DIR_RIGHT)
            {
                turn(-90, robot);
                history_log(INFO, "Action: ACT_TURN executed to right (-90°)");
            }
            else
            {
                turn(90, robot);
                history_log(INFO, "Action: ACT_TURN executed default left (90°)");
            }
            break;

        case ACT_STOP:
            history_log(INFO, "Action: ACT_STOP executed");
            break;

        case ACT_SEARCH:
            history_log(INFO, "Action: ACT_SEARCH executed");
            break;

        case ACT_NONE:
            history_log(INFO, "Action: ACT_NONE - no action performed");
            break;

        default:
            fprintf(stderr, "executor: ERREUR executor() UNKNOWN ACTION : %d\n", cmd_list->cmd[i].action);
            history_log(WARNING, "executor: ERREUR executor() UNKNOWN ACTION");
            return;
        }
    }

    history_log(INFO, "All commands in command_list executed");
}
