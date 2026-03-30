#include "mainWindow.hpp"
#include "botsim.hpp"

SimpleWindow::SimpleWindow(int w, int h, const char *title) : Fl_Window(w, h, title) {

    begin();

    bot = std::make_unique<Botsim>(0, 0, w, h);
    bot -> take_focus();

    end();
    
    resizable(this);
    show();
}

//----------------------------------------------------

SimpleWindow::~SimpleWindow() {
    // peut'être foireux, a vérifier si c'est okay que sa soit vide
}

//----------------------------------------------------

