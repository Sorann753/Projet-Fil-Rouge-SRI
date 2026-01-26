/**
 * @author CHALUMEAUX Victor et BELUSCA Joan
 */

 #include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include "controlMod/menu.h"
#include "configLoader/configLoader.h"
#include "testCases/speechTest.h"
#include "history/history.h"
#include "controlMod/voiceInterface/speech.h"

#include "manualPilot/vocabulary.h"
#include "utils/position.h"
#include "actuator/SimulatorController.h"
#include "manualPilot/cutter.h"
#include "manualPilot/analysis.h"
#include "manualPilot/parser.h"
#include "manualPilot/executor.h"

char *languageValue = NULL;
char *simuOpen = NULL;

RobotPosition my_robot;

char selectMenu(void)
{
    char buffer[16];
    if (!fgets(buffer, sizeof(buffer), stdin))
    {
        return '\0';
    }

    return buffer[0];
}

void homeMenu(void)
{
    char choice;
    bool running = true;

    languageValue = config_loader("config/globalConfig.toml", "langue");
    simuOpen = config_loader("config/globalConfig.toml", "simulation_open_on_start");

    if (strcmp(simuOpen, "on") == 0)
    {
        startSimu();
    }

    init_Simulator(&my_robot);
    history_log(INFO, "homeMenu opening");

    if (!languageValue)
    {
        languageValue = malloc(strlen("en") + 1);
        strcpy(languageValue, "en");
    }

    while (running)
    {
        printf("\n _   _______  _____      _ _           _  \n");
        printf("| | | | ___ \\/  ___|    (_) |         | | \n");
        printf("| | | | |_/ /\\ `--.  ___ _| |__   ___ | |_\n");
        printf("| | | |  __/  `--. \\/ __| | '_ \\ / _ \\| __|\n");
        printf("| |_| | |    /\\__/ /\\__ \\ | |_) | (_) | |_ \n");
        printf(" \\___/\\_|    \\____/ |___/_|_.__/ \\___/ \\__|");
        printf("\n------ HOME ------\n");
        printf("| 1.Control Mode |\n");
        printf("| 2.Languages    |\n");
        printf("| 3.Sim Window   |\n");
        printf("| q.Quit         |\n");
        printf("------------------\n\n");

        choice = selectMenu();

        switch (choice)
        {
        case '1':
            controlMenu();
            break;
        case '2':
            languagesMenu();
            break;
        case '3':
            simulationMenu();
            break;
        case 'q':
            printf("--- EXIT ---\n");
            running = false;
            break;
        default:
            system("clear");
        }
    }

    closeSimu();
    history_close();

    free(languageValue);
    languageValue = NULL;

    free(simuOpen);
    simuOpen = NULL;
}

void controlMenu(void)
{
    char choice;
    bool running = true;

    char userCommand[256] = {0};
    char *speechInput = NULL;
    char log_buffer[1024];

    TreeMap *vocab = NULL;

    command_list cmd_list;
    tokenlist liste;

    while (running)
    {
        printf("\n---- Control ----\n");
        printf("| 1.CLI Mode    |\n");
        printf("| 2.Vocal Mode  |\n");
        printf("| 3.Reset Robot |\n");
        printf("| 0.Return      |\n");
        printf("-----------------\n\n");

        choice = selectMenu();

        if (strcmp(languageValue, "en") == 0)
        {
            vocab = vocabulary_load("config/vocabulary/en.toml");
        }
        else
        {
            vocab = vocabulary_load("config/vocabulary/fr.toml");
        }

        switch (choice)
        {
        case '1':
            printf("Enter command: ");
            if (!fgets(userCommand, sizeof(userCommand), stdin))
            {
                history_log(WARNING, "No CLI input received");
                break;
            }
            userCommand[strcspn(userCommand, "\n")] = '\0';

            snprintf(log_buffer, sizeof(log_buffer), "User Text Input: %s", userCommand);
            history_log(INFO, log_buffer);
            break;

        case '2':
            printf("Give your command.\n");
            speechInput = get_speech(languageValue);

            strncpy(userCommand, speechInput, sizeof(userCommand) - 1);
            userCommand[sizeof(userCommand) - 1] = '\0';

            /*free(speechInput);*/
            speechInput = NULL;

            printf("Vocal Input: %s\n", userCommand);

            snprintf(log_buffer, sizeof(log_buffer), "User Vocal Input: %s", userCommand);
            history_log(INFO, log_buffer);
            break;

        case '3':
            init_Simulator(&my_robot);
            freeTreeMap(&vocab);
            continue;

        case '0':
            freeTreeMap(&vocab);
            running = false;
            continue;

        default:
            system("clear");
            freeTreeMap(&vocab);
            continue;
        }

        if (strlen(userCommand) == 0)
        {
            history_log(WARNING, "Aucune commande reçue");
            freeTreeMap(&vocab);
            break;
        }

        if (!vocab)
        {
            printf("ERREUR: impossible de charger le vocabulaire\n");
            history_log(ERROR, "Impossible de charger le dictionnaire");
            break;
        }

        cmd_list = init_command_list();
        liste = init_cutter();

        cutter(userCommand, &liste);
        analyze(vocab, &liste);

        printf("---------- Analyse ------------\n");
        print_analysis(&liste);
        printf("-------------------------------\n");

        int nb_cmd = parser(&liste, &cmd_list);

        printf("Nombre de commandes: %d\n", nb_cmd);
        for (int i = 0; i < cmd_list.count; i++)
        {
            printf("  cmd[%d]: action=%d, value=%.2f, color=%d, direction=%d\n",
                   i, cmd_list.cmd[i].action, cmd_list.cmd[i].value,
                   cmd_list.cmd[i].color, cmd_list.cmd[i].direction);
        }

        execut_cmd(&cmd_list, &my_robot);

        freeTreeMap(&vocab);
    }
}

void languagesMenu(void)
{
    char choice;
    bool running = true;

    printf(" ------------------------\n");
    printf("| Current Language : %s |\n", languageValue);
    printf(" ------------------------\n\n");

    while (running)
    {
        printf("-- Languages --\n");
        printf("| 1.Français  |\n");
        printf("| 2.English   |\n");
        printf("| 0.Return    |\n");
        printf("---------------\n\n");

        choice = selectMenu();

        switch (choice)
        {
        case '1':
            free(languageValue);
            languageValue = malloc(strlen("fr") + 1);
            strcpy(languageValue, "fr");
            history_log(INFO, "Language set to 'fr' ");
            break;

        case '2':
            free(languageValue);
            languageValue = malloc(strlen("en") + 1);
            strcpy(languageValue, "en");
            history_log(INFO, "Language set to 'en' ");
            break;

        case '0':
            running = false;
            continue;

        default:
            system("clear");
            continue;
        }

        printf(" ----------------------\n");
        printf("| Language set to : %s |\n", languageValue);
        printf(" ----------------------\n\n");
    }
}

void simulationMenu(void)
{
    char choice;
    bool running = true;

    while (running)
    {
        printf("-- Simulation Window --\n");
        printf("| 1.New window        |\n");
        printf("| 2.Close window      |\n");
        printf("| 0.Return            |\n");
        printf("----------------------\n\n");

        choice = selectMenu();

        switch (choice)
        {
        case '1':
            if (strcmp(simuOpen, "on") == 0)
            {
                printf("Previous window still open, close it before creating a new one.\n");
                history_log(WARNING, "Failed to create new simulation window");
            }
            else
            {
                free(simuOpen);
                simuOpen = malloc(strlen("on") + 1);
                strcpy(simuOpen, "on");
                startSimu();
                init_Simulator(&my_robot);
                history_log(INFO, "Creating new simulation window ");
            }
            break;

        case '2':
            free(simuOpen);
            simuOpen = malloc(strlen("off") + 1);
            strcpy(simuOpen, "off");
            closeSimu();
            history_log(INFO, "Closing simulation window ");
            break;

        case '0':
            running = false;
            continue;

        default:
            system("clear");
            continue;
        }
    }
}
