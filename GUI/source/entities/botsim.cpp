#include "entities/botsim.hpp"

Botsim::Botsim(int X, int Y, int W, int H)
    : Fl_Box(X, Y, W, H), inputX(W/2), inputY(H/2)
{}

void Botsim::draw()
{
    fl_color(FL_WHITE);
    fl_rectf(x(), y(), w(), h());

    fl_color(FL_RED);
    fl_pie(x() + inputX, y() + inputY, BOT_SIZE, BOT_SIZE, 0, 90);
}

void Botsim::moveFromKey(int key)
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