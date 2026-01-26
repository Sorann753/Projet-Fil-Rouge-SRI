/**
 * @author Arthus Doriath
 * @date 11/2025
 */

#include "utils/matrix.h"

Matrix initMatrix(const size_t lines, const size_t columns, const size_t typeSize){
    // the third one is here to check for very weird integer overflow
    assert(lines > 0 && columns > 0 && typeSize*lines*columns != 0);

    Matrix newMat = {
        .lines = lines,
        .columns = columns,
        .isValid = false,

        //unknown type, to be interpreted by the user
        .content = malloc(typeSize * lines * columns),
    };

    if(newMat.content != NULL){
        newMat.isValid = true;
    }

    return newMat;
}

Matrix* initMatrixPtr(const size_t lines, const size_t columns, const size_t typeSize){
    // the third one is here to check for very weird integer overflow
    assert(lines > 0 && columns > 0 && typeSize*lines*columns != 0);

    Matrix* matPtr = (Matrix*)malloc(sizeof(Matrix));
    Matrix newMat = initMatrix(lines, columns, typeSize);
    if(!newMat.isValid){
        free(matPtr);
        return NULL;
    }

    // copy the newMat to the containing pointer
    // memcpy is necessary because of the const fields
    memcpy(matPtr, &newMat, sizeof(Matrix)); 

    return matPtr;
}

void freeMatrix(Matrix* M){
    free(M->content);
    M->content = NULL;
    M->isValid = false;
}

size_t matrixGetIndex(const Matrix* const mat, const size_t x, const size_t y){
    assert(mat->isValid);
    assert(y < mat->lines);

    size_t index = y * mat->columns + x;
    assert(index < mat->lines * mat->columns);

    return index;
}

bool isValidAccess(const Matrix* const mat, const size_t x, const size_t y){
    if(mat == NULL) return false;
    if(!mat->isValid) return false;

    if(y >= mat->lines) return false;
    if(x >= mat->columns) return false;
    assert(y < mat->lines);
    
    if(y*mat->columns + x > mat->lines * mat->columns) return false;

    // all verifications cleared, it's valid
    return true;
}