#ifndef MENU_BURGER_HPP
#define MENU_BURGER_HPP

#include <FL/Fl_Group.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Menu_Button.H>
#include <functional>

#include "TCP/client.hpp"
 
class BurgerMenu : public Fl_Group {
public:
    std::function<void()> onToggleOverlay;
    std::function<void()> onQuit;
    std::function<void()> onConnect;
 
    BurgerMenu(int x, int y, int w, int h);
 
private:
    Client client;
    Fl_Button* btn;
    Fl_Menu_Button* menu;
 
    static void btnCB(Fl_Widget*, void* self);
    void openMenu();
};

#endif