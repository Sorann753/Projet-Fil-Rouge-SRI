#include <stdio.h>

#include "testCases/speechTest.h"


void testGetSpeech(void)
{
    printf("----- TEST AUDIO -> TEXTE -----\n");
    const char *result = get_speech();
    printf("Transcription : %s\n", result);
    printf("\n-------------------------------\n");
}