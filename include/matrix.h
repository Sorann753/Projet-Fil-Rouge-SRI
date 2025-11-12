#ifndef MATRIX_H
#define MATRIX_H

#include <stdlib.h>
#include <stdint.h>

typedef uint8_t byte;

typedef struct Matrix_t {
    size_t lines;
    size_t columns;

    void* content;
} Matrix;

Matrix* initMatrix(const size_t lines, const size_t columns, const size_t typeSize);

void freeMatrix(Matrix* matrice);

#endif