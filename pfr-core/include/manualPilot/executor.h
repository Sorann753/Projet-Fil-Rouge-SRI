#ifndef __EXECUTOR_H__
#define __EXECUTOR_H__
#include "manualPilot/parser.h"
#include "actuator/SimulatorController.h"

/**
 * TODO: execute les commandes avec les fonction de simullatorController
 */
void execut_cmd(command_list *cmd_list, RobotPosition *robot);

#endif