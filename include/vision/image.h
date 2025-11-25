#ifndef IMAGE_H
#define IMAGE_H

#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "utils/matrix_impl.h"

#define HEADER_LENGTH 255
#define FIELD_LENGTH 50

typedef struct Pixel_t{
    byte R;
    byte G;
    byte B;
} Pixel;

typedef struct ImageHeader_t{
    size_t length;
    size_t height;
    uint8_t canalCount;
} ImageHeader;

typedef Pixel Color;

typedef Matrix Image; // for Pixel Matrix



ImageHeader parseHeader(const char* rawHeader);

ImageHeader loadHeader(FILE* source);

Image loadImage(const char* sourcePath);

Pixel getPixel(const Image* const img, size_t X, size_t Y);

void setPixel(Image* const img, size_t X, size_t Y, Pixel p);

#endif