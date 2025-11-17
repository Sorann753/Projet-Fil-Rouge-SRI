/**
 * @author Arthus Doriath
 * @date 11/2025
 */

#include "matrix.h"

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
