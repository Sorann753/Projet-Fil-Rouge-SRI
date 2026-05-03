#ifndef CANVASSIM_HPP
#define CANVASSIM_HPP

#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <algorithm>
#include <vector>

#include "entities/entities.hpp"


class CanvasSim : public Fl_Box
{
private:
    struct Polar {
    double r;
    double theta;
};
    int inputX;
    int inputY;
    int SIZE_RATIO= (w()/h())*30;
    int BOT_SIZE = 40;
    int SPEED = 5;
    std::vector<GeneEntities>obj;
    std::vector<Polar> data = {
        {23.4, 0.12}, {145.2, 1.47}, {78.9, 4.21}, {310.5, 2.95},
        {12.7, 5.81}, {220.3, 0.67}, {95.0, 3.33}, {402.1, 1.10},
        {187.6, 4.78}, {54.8, 2.12}, {275.0, 0.33}, {330.4, 5.02},
        {160.9, 3.99}, {7.6, 1.95}, {199.2, 0.89}, {420.0, 2.40},
        {88.3, 5.50}, {143.7, 4.05}, {60.2, 0.05}, {255.8, 3.14},
        {312.6, 2.01}, {48.0, 4.44}, {175.5, 1.22}, {101.1, 0.77},
        {390.7, 5.95}, {132.4, 2.68}, {26.9, 3.80}, {214.0, 4.90},
        {69.5, 1.40}, {298.3, 0.21}, {357.2, 2.57}, {115.6, 5.20},
        {41.7, 0.98}, {186.8, 3.46}, {230.5, 1.58}, {9.9, 2.73},
        {266.4, 5.37}, {147.0, 0.44}, {73.8, 3.05}, {324.9, 4.36},
        {52.5, 1.66}, {208.1, 5.84}, {139.3, 2.25}, {31.2, 0.29},
        {284.6, 3.91}, {372.0, 1.05}, {94.7, 4.58}, {161.8, 0.81},
        {19.5, 5.14}, {249.9, 2.36}
    };
     

    
public:
    CanvasSim(int X, int Y, int W, int H);

    void draw() override;
    void moveFromKey(int key);
};


#endif