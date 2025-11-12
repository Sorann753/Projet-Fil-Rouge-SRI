/**
 * @author Arthus Doriath
 * @date 11/2025
 */

#ifndef MATRIX_IMPL_H
#define MATRIX_IMPL_H

#include "matrix.h"

int int_matrixGet(const Matrix* const mat, const size_t x, const size_t y);

void int_matrixSet(Matrix* const mat, const size_t x, const size_t y, const int value);



double double_matrixGet(const Matrix* const mat, const size_t x, const size_t y);

void double_matrixSet(Matrix* const mat, const size_t x, const size_t y, const double value);

#endif