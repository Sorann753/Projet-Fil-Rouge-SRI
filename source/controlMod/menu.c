#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "controlMod/menu.h"
#include "configLoader/configLoader.h"
#include "testCases/speechTest.h"
#include "history/history.h"
#include "controlMod/voiceInterface/speech.h"


char *languageValue = NULL;

/* Lecture sécurisée d’un choix utilisateur */
char selectMenu(void)
{
    int c;
    do {
        c = getchar();
    } while (c == '\n' || c == '\r');

    return (char)c;
}

void homeMenu(void)
{
   char choice;
   bool running = true;

   history_log(INFO,"homeMenu opening");
   languageValue = config_loader("config/globalConfig.toml", "langue");

   if (!languageValue) {
      languageValue = malloc(strlen("en") + 1);
      strcpy(languageValue, "en");
   }

    while (running) {
        printf("------ HOME ------\n");
        printf("| 1.Control Mode |\n");
        printf("| 2.Languages    |\n");
        printf("| q.Quit         |\n");
        printf("------------------\n\n");

        choice = selectMenu();

        switch (choice) {
            case '1':
                controlMenu();
                break;
            case '2':
                languagesMenu();
                break;
            case 'q':
                printf("--- EXIT ---\n");
                running = false;
                break;
            default:
                system("clear");
        }
    }
    history_close();
    free(languageValue);
    languageValue = NULL;
}

void controlMenu(void)
{
    char choice;
    bool running = true;
    const char *speechResult;
    while (running) {
        printf("---- Control ----\n");
        printf("| 1.CLI Mode    |\n");
        printf("| 2.Vocal Mode  |\n");
        printf("| 0.Return      |\n");
        printf("-----------------\n\n");

        choice = selectMenu();

        switch (choice) {
            case '1':
                printf("CLI MODE WORKING PROGRESS\n");
                WORKING_PROGRESS();
                break;
            case '2':
                speechResult = get_speech(languageValue);
                printf("Transcription : %s\n", speechResult);
                break;
            case '0':
                running = false;
                break;
            default:
                system("clear");
        }
    }
}

void languagesMenu(void)
{
   char choice;
   bool running = true;
   printf(" Language set to : %s\n", languageValue);
   while (running) {
      printf("-- Languages --\n");
      printf("| 1.Français  |\n");
      printf("| 2.English   |\n");
      printf("| 0.Return    |\n");
      printf("---------------\n\n");

      choice = selectMenu();
      
      switch (choice) {
            case '1':
               free(languageValue);
               languageValue = malloc(strlen("fr") + 1);
               strcpy(languageValue, "fr");
               history_log(INFO,"Language set to 'fr' ");
               break;

            case '2':
               free(languageValue);
               languageValue = malloc(strlen("en") + 1);
               strcpy(languageValue, "en");
               history_log(INFO,"Language set to 'en' ");
               break;

            case '0':
               running = false;
               continue;

            default:
               system("clear");
               continue;
      }

      printf("Language set to : %s\n", languageValue);
   }
}


void WORKING_PROGRESS(void)
{
    char choice;
    bool running = true;

    while (running) {
        printf("WORKING_PROGRESS\n");
        printf("press q for exit\n");

        choice = selectMenu();
        if (choice == 'q') {
            running = false;
        }
    }
}
