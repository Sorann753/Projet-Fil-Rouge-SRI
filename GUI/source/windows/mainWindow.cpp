#include "windows/mainWindow.hpp"

SimpleWindow::SimpleWindow(int w, int h, const char *title) : Fl_Window(w, h, title) {

    begin();

    bot = std::make_unique<CanvasSim>(w-w, h-h, w, h);
    burger = new BurgerMenu(w - 44, 4, 36, 32);
    burger->onToggleOverlay = [this]() { toggleOverlay(); };
    burger->onQuit = [](){ exit(0); };
    overlay = new DevOverlay(0, 0, w, h);
    end();
    
    resizable(this);
    show();
}

//----------------------------------------------------

SimpleWindow::~SimpleWindow() {
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

void SimpleWindow::toggleOverlay() {
    overlay->toggle();
    redraw();
}