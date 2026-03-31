#ifndef BOTSIM_HPP
#define BOTSIM_HPP

#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <algorithm>


class Botsim : public Fl_Box
{
private:
    int inputX;
    int inputY;
    int BOT_SIZE = 40;
    int SPEED = 5;
public:
    Botsim(int X, int Y, int W, int H);

    void draw() override;
    void moveFromKey(int key);
};


#endif