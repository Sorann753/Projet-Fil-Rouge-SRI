#ifndef DEVOVERLAY_HPP
#define DEVOVERLAY_HPP

#include <FL/Fl_Widget.H>
#include <cstddef>
#include <FL/fl_draw.H>
#include <cstdio>

#include "utils/utils.hpp"
#include "utils/appData.hpp"


class DevOverlay : public Fl_Widget {
public:
    DevOverlay(int x, int y, int w, int h);
 
    void toggle();
    void draw() override;
    int  handle(int) override;
 
private:
    void drawPanel(int px, int py, int pw, int ph);
    void drawMetrics(int tx, int ty, int lineH);
};



#endif