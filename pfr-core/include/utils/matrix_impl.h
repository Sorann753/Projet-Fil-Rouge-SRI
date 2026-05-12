/**
 * @author Arthus Doriath
 * @date 11/2025
 */

#ifndef MATRIX_IMPL_H
#define MATRIX_IMPL_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h> 

#include "utils/matrix.h"


// NOTE : the speed of the mathematical operations on the matrix may be improved, however it might not be necessary

int int_matrixGet(const Matrix* const mat, size_t x, size_t y);

void int_matrixSet(Matrix* const mat, size_t x, size_t y, int value);

void int_matrixPrint(const Matrix* const mat);

Matrix int_addMatrix(const Matrix* const first, const Matrix* const second);

Matrix int_scaleMatrix(const Matrix* const first, int scalar);

Matrix int_multiplyMatrix(const Matrix* const first, const Matrix* const second);



double double_matrixGet(const Matrix* const mat, size_t x, size_t y);

void double_matrixSet(Matrix* const mat, size_t x, size_t y, double value);

void double_matrixPrint(const Matrix* const mat);

Matrix double_addMatrix(const Matrix* const first, const Matrix* const second);

Matrix double_scaleMatrix(const Matrix* const mat, double scalar);

Matrix double_multiplyMatrix(const Matrix* const first, const Matrix* const second);



byte byte_matrixGet(const Matrix* const mat, size_t x, size_t y);

void byte_matrixSet(Matrix* const mat, size_t x, size_t y, byte value);

void byte_matrixPrint(const Matrix* const mat);

Matrix byte_addMatrix(const Matrix* const first, const Matrix* const second);

Matrix byte_scaleMatrix(const Matrix* const first, byte scalar);

Matrix byte_multiplyMatrix(const Matrix* const first, const Matrix* const second);

#ifdef __cplusplus
}
#endif
#endif