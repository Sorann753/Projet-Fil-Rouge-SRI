/**
 * @author Arthus Doriath
 * @date 11/2025
 */

#include "matrix_impl.h"

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

void int_matrixPrint(const Matrix* const mat){
    assert(mat->isValid);

    size_t blockCount = mat->columns * mat->lines;
    int* matContent = (int*)mat->content;
    for(size_t i = 0; i < blockCount; i++){
        printf("%d ", matContent[i]);
        if(i+1 % mat->columns == 0){
            printf("\n");
        }
    }
}

Matrix int_addMatrix(const Matrix* const first, const Matrix* const second){
    assert(first->isValid && second->isValid);
    // check dimensions
    assert(first->columns == second->columns && first->lines == second->lines);

    // prepare the matrix to store the results
    Matrix result = initMatrix(first->lines, first->columns, sizeof(int));

    // create alias to make code cleaner
    size_t blockCount = first->columns * first->lines;
    int* firstContent = (int*)first->content;
    int* secondContent = (int*)second->content;
    int* resultContent = (int*)result.content;
    
    // NOTE : since the matrix is linearized we can iterate through it with a single loop
    for(size_t i = 0; i < blockCount; i++){
        resultContent[i] = firstContent[i] + secondContent[i];
    }

    // NOTE : ownership is handed back to the caller so be sure to free it later
    return result;
}

Matrix int_scaleMatrix(const Matrix* const mat, const int scalar){
    assert(mat->isValid);
    
    Matrix result = initMatrix(mat->lines, mat->columns, sizeof(int));

    size_t blockCount = mat->lines * mat->columns;
    int* resultContent = (int*)result.content;
    int* matContent = (int*)mat->content;

    for(size_t i = 0; i < blockCount; i++){
        resultContent[i] = matContent[i] * scalar;
    }

    return result;
}

Matrix int_multiplyMatrix(const Matrix* const first, const Matrix* const second){
    assert(first->isValid && second->isValid);
    // dimensions are flipped for a matrix product
    assert(first->lines == second->columns && first->columns == second->lines);

    Matrix result = initMatrix(first->lines, second->columns, sizeof(int));

    int* resultContent = (int*)result.content;
    int* firstContent = (int*)first->content;
    int* secondContent = (int*)second->content;

    // NOTE : this is a simple implementation so it has O(n^3) complexity, upgrade if needed later
    for(size_t i = 0; i < first->lines; i++){
        for(size_t j = 0; j < second->columns; j++){
            size_t resIndex = i * second->columns + j;
            resultContent[resIndex] = 0;
            for(size_t k = 0; k < first->columns; k++){
                size_t leftIndex = i * first->columns + k;
                size_t rightIndex = k * second->columns + j;
                resultContent[resIndex] += firstContent[leftIndex] * secondContent[rightIndex];
            }
        }
    }

    return result;
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

void double_matrixPrint(const Matrix* const mat){
    assert(mat->isValid);

    size_t blockCount = mat->columns * mat->lines;
    double* matContent = (double*)mat->content;
    for(size_t i = 0; i < blockCount; i++){
        printf("%f ", matContent[i]);
        if((i+1) % mat->columns == 0){
            printf("\n");
        }
    }
}

Matrix double_addMatrix(const Matrix* const first, const Matrix* const second){
    assert(first->isValid && second->isValid);
    // check dimensions
    assert(first->columns == second->columns && first->lines == second->lines);

    // prepare the matrix to store the results
    Matrix result = initMatrix(first->lines, first->columns, sizeof(double));

    // create alias to make code cleaner
    size_t blockCount = first->columns * first->lines;
    double* firstContent = (double*)first->content;
    double* secondContent = (double*)second->content;
    double* resultContent = (double*)result.content;
    
    // NOTE : since the matrix is linearized we can iterate through it with a single loop
    for(size_t i = 0; i < blockCount; i++){
        resultContent[i] = firstContent[i] + secondContent[i];
    }

    // NOTE : ownership is handed back to the caller so be sure to free it later
    return result;
}

Matrix double_scaleMatrix(const Matrix* const mat, const double scalar){
    assert(mat->isValid);
    
    Matrix result = initMatrix(mat->lines, mat->columns, sizeof(double));

    size_t blockCount = mat->lines * mat->columns;
    double* resultContent = (double*)result.content;
    double* matContent = (double*)mat->content;

    for(size_t i = 0; i < blockCount; i++){
        resultContent[i] = matContent[i] * scalar;
    }

    return result;
}

Matrix double_multiplyMatrix(const Matrix* const first, const Matrix* const second){
    assert(first->isValid && second->isValid);
    // dimensions are flipped for a matrix product
    assert(first->lines == second->columns && first->columns == second->lines);

    Matrix result = initMatrix(first->lines, second->columns, sizeof(double));

    double* resultContent = (double*)result.content;
    double* firstContent = (double*)first->content;
    double* secondContent = (double*)second->content;

    // NOTE : this is a simple implementation so it has O(n^3) complexity, upgrade if needed later
    for(size_t i = 0; i < first->lines; i++){
        for(size_t j = 0; j < second->columns; j++){
            size_t resIndex = i * second->columns + j;
            resultContent[resIndex] = 0;
            for(size_t k = 0; k < first->columns; k++){
                size_t leftIndex = i * first->columns + k;
                size_t rightIndex = k * second->columns + j;
                resultContent[resIndex] += firstContent[leftIndex] * secondContent[rightIndex];
            }
        }
    }

    return result;
}
