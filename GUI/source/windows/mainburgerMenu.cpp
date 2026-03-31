#include "windows/burgerMenu.hpp"

BurgerMenu::BurgerMenu(int X, int Y, int W, int H, const char *title) : Fl_Menu_Button(X, Y, W, H, title)
{
    add("Test1");
    add("Test2");
    
}

BurgerMenu::~BurgerMenu()
{
}