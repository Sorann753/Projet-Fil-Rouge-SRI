#include "entities/canvasSim.hpp"
#include <iostream>
#include <iterator>
#include "utils/utils.hpp"

CanvasSim::CanvasSim(int X, int Y, int W, int H)
    : Fl_Box(X, Y, W, H), inputX(W/2), inputY(H/2)
{}


void CanvasSim::draw()
{
    // set robot

    fl_color(FL_WHITE);
    fl_rectf(x(), y(), w(), h());

    fl_color(FL_RED);
    fl_pie(x() + inputX, y() + inputY, BOT_SIZE, BOT_SIZE, -130, -50);

    //

    // Entities generation
    /*for (int i = 0; i <data.size() ; i++)
    {
        auto tabs = GeneEntities(500+i,50+i,SIZE_RATIO,50);
        obj.push_back(tabs);
    }*/
    //
        for (const auto& p : data) {
            coordinate coo = polarConvert(p.r,p.theta);
            auto tabs = GeneEntities(coo.x+(w()/2),coo.y+(h()/2),SIZE_RATIO,SIZE_RATIO);
    }

}

void CanvasSim::moveFromKey(int key)
{
    int minX = 0, maxX = w() - BOT_SIZE;
    int minY = 0, maxY = h() - BOT_SIZE;

    switch (key)
    {
        case 'z': inputY = std::max(minY, inputY - SPEED); break;
        case 's': inputY = std::min(maxY, inputY + SPEED); break;
        case 'q': inputX = std::max(minX, inputX - SPEED); break;
        case 'd': inputX = std::min(maxX, inputX + SPEED); break;
        default: return; 
    }

    redraw();
}