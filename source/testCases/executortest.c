#include <stdio.h>
#include "manualPilot/parser.h"
#include "manualPilot/cutter.h"
#include "manualPilot/vocabulary.h"
#include "manualPilot/analysis.h"
#include "manualPilot/executor.h"
#include "configLoader/configLoader.h"

void test_executor(const char *phrase)
{
    printf("---- TEST MANUAL PILOTE ----\n\n");

    char *python_path = config_loader("config/globalConfig.toml", "python_simulation_path");

    /*charger le vocabulaire */
    TreeMap *vocab = vocabulary_load("config/vocabulary/fr.toml");
    if (vocab == NULL)
    {
        printf("ERREUR: impossible de charger le vocabulaire\n");
        return;
    }

    /*initialisation de la position du robot*/
    RobotPosition my_robot;

    /*initialisation du fichier SimulatorController.txt*/
    init_Simulator(&my_robot);

    /*phprase de test*/

    printf("Phrase: \"%s\"\n\n", phrase);

    /* découper la phrase */
    tokenlist liste = init_cutter();
    cutter(phrase, &liste);

    printf("\nAVANT ANALYSE:\n");
    print_tokenlist(&liste);

    /* analyser les tokens */
    analyze(vocab, &liste);

    printf("\nAPRES ANALYSE:");
    print_analysis(&liste);

    command_list cmd_list = init_command_list();
    int nb_cmd = parser(&liste, &cmd_list);

    printf("\nnombre de commandes: %d\n", nb_cmd);
    for (int i = 0; i < cmd_list.count; i++)
    {
        printf("  cmd[%d]: action=%d, value=%.2f, color=%d, direction=%d\n", i, cmd_list.cmd[i].action, cmd_list.cmd[i].value, cmd_list.cmd[i].color, cmd_list.cmd[i].direction);
    }

    printf("\nexecution des commandes\n");
    execut_cmd(&cmd_list, &my_robot);

    printf("\nCONTENU DE SimulatorController.txt :\n");
    system("cat ./SimulatorController.txt");
    printf("\n");

    /* creation d'une variable pour trouver le chemin du script python a executer*/
    char full_python_path[512];
    snprintf(full_python_path, sizeof(full_python_path), "../../../../%s", python_path);

    /*definir la commande a executer dans le terminale*/
    char commande[600];

    /*ecrire dans commande*/
    sprintf(commande, "python3 %s", full_python_path);

    /*lancer la simulation*/
    system(commande);

    /* libérer la mémoire */
    freeTreeMap(&vocab);
}