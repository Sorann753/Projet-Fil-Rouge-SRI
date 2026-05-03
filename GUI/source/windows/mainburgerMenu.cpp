#include "windows/burgerMenu.hpp"

BurgerMenu::BurgerMenu(int x, int y, int bw, int bh)
    : Fl_Group(x, y, bw, bh)
{
    // Bouton ☰
    btn = new Fl_Button(x, y, bw, bh, "@#menu");
    btn->box(FL_FLAT_BOX);
    btn->color(fl_rgb_color(30, 30, 40));
    btn->labelcolor(fl_rgb_color(200, 200, 220));
    btn->labelsize(20);
    btn->tooltip("Menu");
    btn->callback(btnCB, this);

    // Menu popup (taille nulle, affiché manuellement)
    menu = new Fl_Menu_Button(0, 0, 0, 0);
    menu->type(Fl_Menu_Button::POPUP3);

    menu->add("Dev Overlay\t(toggle)", 0,
        [](Fl_Widget*, void* self) {
            auto* bm = static_cast<BurgerMenu*>(self);
            if (bm->onToggleOverlay) bm->onToggleOverlay();
        }, this);

    menu->add("Quitter", 0,
        [](Fl_Widget*, void* self) {
            auto* bm = static_cast<BurgerMenu*>(self);
            if (bm->onQuit) bm->onQuit();
        }, this);

    end();
}

void BurgerMenu::btnCB(Fl_Widget*, void* self) {
    static_cast<BurgerMenu*>(self)->openMenu();
}

void BurgerMenu::openMenu() {
    menu->position(btn->x(), btn->y() + btn->h());
    menu->popup();
}