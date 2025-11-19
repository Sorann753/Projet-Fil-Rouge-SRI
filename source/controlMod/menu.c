#include <stdlib.h>

#include "menu.h"

char selectMenu(void)
{
   int character;
   character = getchar();
   return character;
}

 void homeMenu()
 {
   char choice;
   do
   {
      system("clear");
      printf("HOME\n");
      printf("1.Control Mode\n");
      printf("2.Languages\n");
      printf("q.Quit\n");
      choice = selectMenu();
      switch (choice)
      {
      case 49:
         controlMenu();
         break;
      case 50:
         languagesMenu();
         break;
      case 113:
         printf("EXIT");
         break;
      default:
         printf("invalide choice please retry\n");
      }
   } while (choice != 113);

 }

 void controlMenu()
 {
   int choice;
   do
   {
      system("clear");
      printf("Control\n");
      printf("1.CLI Mode\n");
      printf("2.Vocal Mode\n");
      printf("0.Return\n");
      choice = selectMenu();
      switch (choice)
      {
      case 49:
         printf("CLI MODE WORKING PROGRESS");
         break;
      case 50:
         printf("VOCAL MODE WORKING PROGRESS");
         break;
      case 48:
         homeMenu();
         break;
      default:
         printf("invalide choice please retry\n");
      }
   } while (choice != 48);
   
 }

 void languagesMenu()
{
   char choice;
   do
   {
      system("clear");
      printf("Languages\n");
      printf("1.Français\n");
      printf("2.English\n");
      printf("0.Return\n");
      choice = selectMenu();
      switch (choice)
      {
      case 49:
         printf("FRANÇAIS WORKING PROGRESS");
         break;
      case 50:
         printf("ENGLISH WORKING PROGRESS");
         break;
      case 48:
         homeMenu();
         break;
      default:
         printf("invalide choice please retry\n");
      }
   } while (choice != 48);
}
