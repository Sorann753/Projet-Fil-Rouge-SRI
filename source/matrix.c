/**
 * @author Arthus Doriath
 * @date 11/2025
 */

#include "matrix.h"

Matrix initMatrix(const size_t lines, const size_t columns, const size_t typeSize){
    Matrix newMat = {
        .lines = lines,
        .columns = columns,

        //unknown type, to be interpreted by the user
        .content = malloc(typeSize * lines * columns),
    };

    return newMat;
}

void freeMatrix(Matrix* M){
    free(M->content);
    M->content = NULL;
}

size_t matrixGetIndex(const Matrix* const mat, const size_t x, const size_t y){
    assert(y < mat->lines);

    size_t index = y * mat->columns + x;
    assert(index < mat->lines * mat->columns);

    return index;
}
