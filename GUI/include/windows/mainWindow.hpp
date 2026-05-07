#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Output.H>
#include <memory>

#include"windows/devOverlay.hpp"
#include "entities/canvasSim.hpp"
#include "entities/entities.hpp"
#include "TCP/client.hpp"


class SimpleWindow : public Fl_Window {

public:
    SimpleWindow(int w, int h, const char *title);
    ~SimpleWindow();

    int handle(int event)override;
    std::unique_ptr<CanvasSim> bot;
    Client* comm;
    
private:

    //static void cb_copy(Fl_Widget *, void *);
    //inline void cb_copy_i();

    //static void cb_quit(Fl_Widget *, void *);
    //inline void cb_quit_i();
};

#endif