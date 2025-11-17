#include "menu.h"

 void homeMenu()
 {
   printf("HOME\n");
   printf("1.Control Mode\n");
   printf("2.Languages\n");
   printf("0.Quit\n");
 }

 void controlMenu()
 {
   printf("Control\n");
   printf("1.CLI Mode\n");
   printf("2.Vocal Mode\n");
   printf("0.Return\n");
 }

 void languagesMenu()
{
   printf("Languages\n");
   printf("FR.Français\n");
   printf("ENG.English\n");
   printf("0.Return\n");
}


void menu()
{
   int character;
   homeMenu();
   character=getchar();
   switch (character)
   {
   case 0:
      break;
   case 1:
      printf("choix 1");
      controlMenu();
   }
   printf("hors");
   


}
