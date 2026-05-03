#ifndef DEVOVERLAY_HPP
#define DEVOVERLAY_HPP

#include <FL/Fl_Widget.H>

enum State {
    IDLE,
    MOVING,
    EMERGENCY
};

struct AppData
{
    State currentState;
    int entityCount;
    bool devOverlayVisible = false;

};

class DevOverlay : public Fl_Widget {
public:
    DevOverlay(int x, int y, int w, int h);
 
    // Active / désactive l'overlay
    void toggle();
 
protected:
    void draw()        override;
    int  handle(int)   override;   // passe-travers : ne consomme aucun événement
 
private:
    void drawPanel(int px, int py, int pw, int ph);
    void drawMetrics(int tx, int ty, int lineH);
};



#endif