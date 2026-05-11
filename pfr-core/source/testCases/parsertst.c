#include <stdio.h>
#include "manualPilot/parser.h"
#include "manualPilot/cutter.h"
#include "manualPilot/vocabulary.h"
#include "manualPilot/analysis.h"

void test_parser(void)
{
    printf("---- TEST PARSER ----\n\n");

    /*charger le vocabulaire */
    TreeMap *vocab = vocabulary_load("config/vocabulary/fr.toml");
    if (vocab == NULL)
    {
        printf("ERREUR: impossible de charger le vocabulaire\n");
        return;
    }

    /*phprase de test*/
    const char *phrase = "Avance de 100 metres puis tourne a gauche et recule de 30 metres et cherche le cube rouge";

    printf("Phrase: \"%s\"\n\n", phrase);

    /* découper la phrase */
    tokenlist liste = init_cutter();
    cutter(phrase, &liste);

    /* analyser les tokens */
    analyze(vocab, &liste);
    printf("TOKENS ANALYSÉS:\n");
    print_analysis(&liste);

    command_list cmd_list = init_command_list();
    int nb_cmd = parser(&liste, &cmd_list);

    printf("nombre de commandes: %d\n", nb_cmd);
    for (int i = 0; i < cmd_list.count; i++)
    {
        printf("  cmd[%d]: action=%d, value=%.2f, color=%d, direction=%d\n", i, cmd_list.cmd[i].action, cmd_list.cmd[i].value, cmd_list.cmd[i].color, cmd_list.cmd[i].direction);
    }

    /* libérer la mémoire */
    freeTreeMap(&vocab);
}