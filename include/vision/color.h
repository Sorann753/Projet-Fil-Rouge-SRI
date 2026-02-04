#ifndef VISION_COLOR_H
#define VISION_COLOR_H

#include <stdio.h>

#include "image.h"

typedef Pixel Color; // cuz they are basically the same thing

typedef struct HSVColor_t{
    float H;
    float S;
    float V;
} HSVColor;

typedef struct ColorReferences_t{
    Color* refPoints;
    uint8_t colorCount;
} ColorReferences;

typedef struct ColorMasks_t{
    Matrix* masks;
    uint8_t colorCount;
} ColorMasks;

/**
 * @brief convert the standard RGB color to the HSV color system
 */
HSVColor RGBtoHSV(Color base);

/**
 * @warning this was not implemented because this was not necessary yet
 */
Color HSVtoRGB(HSVColor base);

/**
 * @brief estimate the distance between A and B as if they were points on a cylindrical space
 * @note H is a pure angle so it's in the Lie group U(1) and therefore the radius
 * isn't taken into accound for this curved distance
 */
double colorDistance(HSVColor A, HSVColor B);

ColorMasks extractColors(const Image* img, const ColorReferences colorRef);

void freeColorMasks(ColorMasks* masks);

/**
 * @brief reconstruct an image file from the color masks for visualization purposes
 */
bool exportMasks(const char* outFilePath, const ColorMasks masks, const ColorReferences ref);

#endif