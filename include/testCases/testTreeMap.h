#ifndef TEST_TREE_MAP_H
#define TEST_TREE_MAP_H

#include <assert.h>
#include <stdio.h>

#include "utils/treeMap.h"

typedef struct testObject_t {
    char* evenAPoiter;
    double someDouble;
    int someInt;
    bool oneBool;
} testObject;


void testTreeMap(void);

const char* fakeKey(unsigned int i);

void testTreeManyInsert(int insertCount);

#endif