#include "botsim.hpp"

Botsim::Botsim(int X, int Y, int W, int H)
    : Fl_Box(X, Y, W, H), inputX(1500/2), inputY(900/2)
{
}

void Botsim::draw()
{
    fl_color(FL_WHITE);
    fl_rectf(x(), y(), w(), h());

    fl_color(FL_RED);
    fl_pie(x() + inputX, y() + inputY, BOT_SIZE, BOT_SIZE, 0, 90);
}

int Botsim::handle(int event)
{
    switch (event)
    {
        case FL_FOCUS:
        case FL_UNFOCUS:
            return 1;

        case FL_KEYDOWN:
        {
            int minX = 0, maxX = w() - BOT_SIZE;
            int minY = 0, maxY = h() - BOT_SIZE;

            switch (Fl::event_key())
            {
                case 'z': inputY = std::max(minY, inputY - SPEED); break;
                case 's': inputY = std::min(maxY, inputY + SPEED); break;
                case 'q': inputX = std::max(minX, inputX - SPEED); break;
                case 'd': inputX = std::min(maxX, inputX + SPEED); break;
                default: return Fl_Box::handle(event);
            }

            redraw();
            return 1;
        }
    }

    return Fl_Box::handle(event);
}