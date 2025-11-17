/**
 * @author Arthus Doriath
 * @date 11/2025
 */

#ifndef MATRIX_IMPL_H
#define MATRIX_IMPL_H

#include "matrix.h"


// NOTE : the speed of the mathematical operations on the matrix may be improved, however it might not be necessary

int int_matrixGet(const Matrix* const mat, const size_t x, const size_t y);

void int_matrixSet(Matrix* const mat, const size_t x, const size_t y, const int value);

Matrix int_addMatrix(const Matrix* const first, const Matrix* const second);

Matrix int_scaleMatrix(const Matrix* const first, const int scalar);

Matrix int_multiplyMatrix(const Matrix* const first, const Matrix* const second);



double double_matrixGet(const Matrix* const mat, const size_t x, const size_t y);

void double_matrixSet(Matrix* const mat, const size_t x, const size_t y, const double value);

Matrix double_addMatrix(const Matrix* const first, const Matrix* const second);

Matrix double_scaleMatrix(const Matrix* const mat, const double scalar);

Matrix double_multiplyMatrix(const Matrix* const first, const Matrix* const second);

#endif