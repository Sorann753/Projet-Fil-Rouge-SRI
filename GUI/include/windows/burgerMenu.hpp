#ifndef MENU_BURGER_HPP
#define MENU_BURGER_HPP

#include <FL/Fl_Menu_Button.H>

class BurgerMenu : public Fl_Menu_Button
{
public:
    BurgerMenu(int X, int Y, int W, int H, const char *title);
    ~BurgerMenu();
};

#endif