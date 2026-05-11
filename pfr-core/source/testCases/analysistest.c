#include <stdio.h>
#include "manualPilot/cutter.h"
#include "manualPilot/vocabulary.h"
#include "manualPilot/analysis.h"

void test_analysis(void)
{
    printf("---- TEST ANALYSIS ----\n\n");

    /* 1. Charger le vocabulaire */
    TreeMap *vocab = vocabulary_load("config/vocabulary/fr.toml");
    if (vocab == NULL)
    {
        printf("ERREUR: impossible de charger le vocabulaire\n");
        return;
    }

    /* Découper une phrase */
    tokenlist liste = init_cutter();
    cutter("Avance de 50 metres et tourne a gauche", &liste);

    printf("AVANT analyse:\n");
    print_tokenlist(&liste);

    /* 3. Analyser les tokens */
    analyze(vocab, &liste);

    printf("\nAPRÈS analyse:\n");
    print_analysis(&liste);

    /* 4. Libérer la mémoire */
    freeTreeMap(&vocab);
}