#ifndef IMAGE_H
#define IMAGE_H

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <math.h>

#include "utils/matrix_impl.h"

#define HEADER_MAX_LENGTH 255
#define FIELD_MAX_LENGTH 16
#define IMG_BUFFER_SIZE 1500
#define NAK (char)0x15

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

typedef Pixel Color; // cuz they are basically the same thing

// the type for a Matrix of Pixel
typedef Matrix Image;

typedef enum ParsingState_t{
    IMG_STATE_READ, // looking for the next field
    IMG_STATE_CUT, // currently in the middle of a field
    IMG_STATE_STOP // terminate
} ParsingState;



ImageHeader parseHeader(const char* rawHeader);

ImageHeader loadHeader(FILE* source);

void getChunk(FILE* source, char* buffer, size_t bufferSize, size_t remaining);

/**
 * @note this function assume ASCII compatible files,
 * other characters may be read weirdly but won't crash anything (unlike scanf)
 */
Matrix channelLexer(FILE* source, ImageHeader head);

/**
 *
 * @note this function assume 3 channel images
 */
void readImage(FILE* source, Image* img);

/**
 * @brief load an image in RBG-txt format
 * @param sourcePath a valid path to the file we need to read
 * @return a valid Image encoded as a Matrix of Pixel if the reading went well

 * OR
 * 
 * an invalid Image if any error occured, in which case the errno value shall be checked
 * and the Image not be used for anything
 */
Image loadImage(const char* sourcePath);



Pixel getPixel(const Image* const img, size_t X, size_t Y);

void setPixel(Image* const img, size_t X, size_t Y, Pixel p);

Image convolut(const Image* img, const Matrix byteMatrix);

// functions for potential vectorization of some operations
// might not be useful but at least it's around if needed
uint32_t packPixel(Pixel rawPixel);
Pixel unpackPixel(uint32_t packedPixel);

#endif