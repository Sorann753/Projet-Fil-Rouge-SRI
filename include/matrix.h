/**
 * @author Arthus Doriath
 * @date 11/2025
 */

#ifndef MATRIX_H
#define MATRIX_H

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

typedef uint8_t byte;

typedef struct Matrix_t {
    const size_t lines;
    const size_t columns;

    void* content;
} Matrix;

Matrix initMatrix(const size_t lines, const size_t columns, const size_t typeSize);

void freeMatrix(Matrix* matrice);

size_t matrixGetIndex(const Matrix* const mat, const size_t x, const size_t y);

#endif