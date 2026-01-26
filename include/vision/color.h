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

HSVColor RGBtoHSV(Color base);

Color HSVtoRGB(HSVColor base);

double colorDistance(HSVColor A, HSVColor B);

ColorMasks extractColors(const Image* img, const ColorReferences colorRef);

void freeColorMasks(ColorMasks* masks);

/**
 * @brief reconstruct an image file from the color masks for visualization purposes
 */
bool exportMasks(const char* outFilePath, const ColorMasks masks, const ColorReferences ref);

#endif