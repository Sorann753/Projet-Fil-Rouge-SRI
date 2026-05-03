#ifndef MENU_BURGER_HPP
#define MENU_BURGER_HPP

#include <FL/Fl_Group.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Menu_Button.H>
#include <functional>
 
class BurgerMenu : public Fl_Group {
public:
    // Callbacks éditables de l'extérieur
    std::function<void()> onToggleOverlay;
    std::function<void()> onQuit;
 
    BurgerMenu(int x, int y, int w, int h);
 
private:
    Fl_Button*      btn;
    Fl_Menu_Button* menu;
 
    static void btnCB(Fl_Widget*, void* self);
    void        openMenu();
};

#endif