#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Output.H>

#include <burgerMenu.hpp>
#include <botsim.hpp>

#include <memory>

class SimpleWindow : public Fl_Window {

public:
    SimpleWindow(int w, int h, const char *title);
    ~SimpleWindow();
      std::unique_ptr<Botsim> bot;
    
private:
    //static void cb_copy(Fl_Widget *, void *);
    //inline void cb_copy_i();

    //static void cb_quit(Fl_Widget *, void *);
    //inline void cb_quit_i();
};

#endif