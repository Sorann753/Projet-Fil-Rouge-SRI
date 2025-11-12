/**
 * @author Arthus Doriath
 * @date 11/2025
 */

#include "matrix.h"

int int_matrixGet(const Matrix* const mat, const size_t x, const size_t y){
    size_t index = matrixGetIndex(mat, x, y);
    int* ptr = (int*)mat->content;
    int value = ptr[index];

    return value;
}

void int_matrixSet(Matrix* const mat, const size_t x, const size_t y, const int value){
    size_t index = matrixGetIndex(mat, x, y);
    int* ptr = ((int*)(mat->content));
    ptr[index] = value;
}



double double_matrixGet(const Matrix* const mat, const size_t x, const size_t y){
    size_t index = matrixGetIndex(mat, x, y);
    double* ptr = (double*)mat->content;
    double value = ptr[index];

    return value;
}

void double_matrixSet(Matrix* const mat, const size_t x, const size_t y, const double value){
    size_t index = matrixGetIndex(mat, x, y);
    double* ptr = ((double*)(mat->content));
    ptr[index] = value;
}
