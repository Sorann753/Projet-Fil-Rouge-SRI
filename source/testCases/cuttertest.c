#include <stdio.h>
#include "manualPilot/cutter.h"

void test_cutter(void)
{
    printf("--- TEST CUTTER --- \n\n");

    tokenlist liste;

    liste = init_cutter();
    cutter("Avance de 50 metres et tourne a gauche", &liste);
    print_tokenlist(&liste);
    printf("\n");
}