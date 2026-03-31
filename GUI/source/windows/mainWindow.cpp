#include <FL/Fl_Box.H>
#include "windows/mainWindow.hpp"
#include "entities/botsim.hpp"

SimpleWindow::SimpleWindow(int w, int h, const char *title) : Fl_Window(w, h, title) {

    begin();

    bot = std::make_unique<Botsim>(0, 0, w, h);
    end();
    
    resizable(this);
    show();
}

//----------------------------------------------------

SimpleWindow::~SimpleWindow() {
    // peut'être foireux, a vérifier si c'est okay que sa soit vide
}

//----------------------------------------------------

int SimpleWindow::handle(int event)

{
    if (event == FL_KEYDOWN)
    {
        bot->moveFromKey(Fl::event_key());
        return 1;
    }
    return Fl_Window::handle(event);
}