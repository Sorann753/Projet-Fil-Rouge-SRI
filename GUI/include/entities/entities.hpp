#ifndef ENTITIES_HPP
#define ENTITIES_HPP

#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <utility>

class GeneEntities : public Fl_Box
{
public:
    GeneEntities(const GeneEntities& other)
        : Fl_Box(other.x(), other.y(), other.w(), other.h())
    {}

    GeneEntities(GeneEntities&&) = delete;
    GeneEntities& operator=(const GeneEntities&) = default;
    GeneEntities& operator=(GeneEntities&&) = default;

    GeneEntities(int x, int y, int w, int h);

    void clearEntities();
};
#endif