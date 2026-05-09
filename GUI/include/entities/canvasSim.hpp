#ifndef CANVASSIM_HPP
#define CANVASSIM_HPP

#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <algorithm>
#include <vector>
#include <iostream>
#include <iterator>

#include "utils/appData.hpp"
#include "entities/entities.hpp"
#include "windows/burgerMenu.hpp"


class CanvasSim : public Fl_Box
{
private:

    int inputX;
    int inputY;
    int SIZE_RATIO= (w()/h())*30;
    int BOT_SIZE = 40;
    int SPEED = 5;
    std::vector<GeneEntities>obj;
    void toggleOverlay();
     

    
public:

    BurgerMenu* burger;
    DevOverlay* overlay;
    CanvasSim(int X, int Y, int W, int H);

    void sizeData(std::vector<Polar> Poldata);
    void draw() override;
    void moveFromKey(int key);
};


#endif