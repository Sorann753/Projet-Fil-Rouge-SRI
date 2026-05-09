#include "entities/entities.hpp"

GeneEntities::GeneEntities(int x,int y, int W, int H)
    : Fl_Box(x, y, W, H)
{
    fl_color(FL_BLUE);
    fl_pie(x,y,W,H,0,360);
}

void GeneEntities::clearEntities()
{
    fl_color(FL_WHITE);
    fl_pie(x(),y(),w(),h(),0,360);
}

