#include "mainWindow.hpp"

SimpleWindow::SimpleWindow(int w, int h, const char *title) : Fl_Window(w, h, title) {

    begin();
    copy = std::make_unique<Fl_Button>(10, 150, 70, 30, "C&opy");
    copy->callback(cb_copy, this);

    quit = std::make_unique<Fl_Button>(100, 150, 70, 30, "&Quit");
    quit->callback(cb_quit, this);

    inp = std::make_unique<Fl_Input>(50, 50, 140, 30, "Input:");
    out = std::make_unique<Fl_Output>(50, 100, 140, 30, "Output:");
    end();

    resizable(this);
    show();
}

//----------------------------------------------------

SimpleWindow::~SimpleWindow() {
    // peut'être foireux, a vérifier si c'est okay que sa soit vide
}

//----------------------------------------------------

void SimpleWindow::cb_copy(Fl_Widget *o, void *v) {

    // or just the one line below
    ((SimpleWindow *)v)->cb_copy_i();
}

void SimpleWindow::cb_copy_i() {

    out->value(inp->value());
}

//----------------------------------------------------

void SimpleWindow::cb_quit(Fl_Widget *, void *v) {

    ((SimpleWindow *)v)->cb_quit_i();
}

void SimpleWindow::cb_quit_i() {

    hide();
}