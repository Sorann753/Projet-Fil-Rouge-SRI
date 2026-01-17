#ifndef TEST_VISION_H
#define TEST_VISION_H

#include "vision/image.h"

typedef struct ImageFingerPint_t {
    size_t lines;
    size_t columns;

    Pixel startPixel;
    Pixel endPixel;

    Pixel Xpixel; // the pixel at coordinate (5, 0)
    Pixel Ypixel; // the pixel at coordinate (0, 5)
} ImageFingerPrint;

void testHeaderParsing(const char* testHeader, const size_t expectedLength, const size_t expectedHeight, const uint8_t expectedCanal);

void testImageLoading(const char* testImagePath, const ImageFingerPrint info);

#endif