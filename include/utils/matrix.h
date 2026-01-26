/**
 * @author Arthus Doriath
 * @date 11/2025
 */

#ifndef MATRIX_H
#define MATRIX_H

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <memory.h>

typedef uint8_t byte;

typedef struct Matrix_t {
    void* content;

    const size_t lines;
    const size_t columns;

    bool isValid;
} Matrix;

#define ERROR_MATRIX (Matrix){NULL, 0, 0, false}

Matrix initMatrix(const size_t lines, const size_t columns, const size_t typeSize);

Matrix* initMatrixPtr(const size_t lines, const size_t columns, const size_t typeSize);

void freeMatrix(Matrix* matrice);

size_t matrixGetIndex(const Matrix* const mat, const size_t x, const size_t y);

bool isValidAccess(const Matrix* const mat, const size_t x, const size_t y);

#endif