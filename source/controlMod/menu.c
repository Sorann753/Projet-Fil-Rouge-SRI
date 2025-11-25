#include <stdlib.h>
#include <stdbool.h>

#include "controlMod/menu.h"

char selectMenu(void)
{
   int character;
   character = getchar();
   return character;
}

void homeMenu()
{
   char choice;
   bool running = true;

   while(running)
   {
      printf("------ HOME ------\n");
      printf("| 1.Control Mode |\n");
      printf("| 2.Languages    |\n");
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
         case 'q':
            printf("--- EXIT ---\n");
            running = false;
            break;
         default:
            system("clear");
      }
   }
}

void controlMenu()
{
   int choice;
   bool running = true;
   
   while(running)
   {
      printf("---- Control ----\n");
      printf("| 1.CLI Mode    |\n");
      printf("| 2.Vocal Mode  |\n");
      printf("| 0.Return      |\n");
      printf("-----------------\n\n");
      choice = selectMenu();
      
      
      switch (choice)
      {
         case '1':
            printf("CLI MODE WORKING PROGRESS\n");
            WORKING_PROGRESS();
            break;
         case '2':
            printf("VOCAL MODE WORKING PROGRESS\n");
            WORKING_PROGRESS();
            break;
         case '0':
            running = false;
            break;
         default:
            system("clear");
      }
   }
}

void languagesMenu()
{
   char choice;
   bool running = true;
   
   while(running)
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
            printf("FRANÇAIS WORKING PROGRESS\n");
            WORKING_PROGRESS();
            break;
         case '2':
            printf("ENGLISH WORKING PROGRESS\n");
            WORKING_PROGRESS();
            break;
         case '0':
            running = false;
            break;
         default:
            system("clear");
      }
   }
}

void WORKING_PROGRESS()
{
   char choice;
   bool running = true;

   while (running)
   {
      printf("WORKING_PROGRESS\n");
      printf("press q for exit\n");
      choice = selectMenu();
      if (choice == 113)
      {
         running = false;
      }
      
   }
   
}