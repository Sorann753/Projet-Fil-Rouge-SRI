#include "entities/entities.hpp"

GeneEntities::GeneEntities(int x,int y, int W, int H)
    : Fl_Box(x, y, W, H)
{
    fl_color(FL_BLUE);
    fl_circle(x,y,W);
}

void GeneEntities::clearEntities()
{
    fl_color(FL_WHITE);
    fl_rectf(x(), y(), w(),h());
}

