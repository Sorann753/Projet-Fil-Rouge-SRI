#include "testCases/testTreeMap.h"

void testTreeMap(void){
    printf("----- TEST TREEMAP USAGE -----\n");

    TreeMap* dict = initTreeMap();

    assert(isInTree(dict, "testWord") == false);
    assert(isInTree(dict, "otherTest") == false);

    testObject* someTestObject = (testObject*)malloc(sizeof(testObject));

    insertValue(&dict, makeKey("testWord"), (void*)someTestObject, false);
    assert(isInTree(dict, "testWord") == true);
    assert(isInTree(dict, "otherTest") == false);

    const testObject* recover = getValue(dict, "testWord");

    // both should be pointing to the same address
    assert(recover == someTestObject);

    testObject* someOtherTestObject = (testObject*)malloc(sizeof(testObject));

    // replace the value in the tree
    insertValue(&dict, makeKey("testWord"), (void*)someOtherTestObject, false);
    assert(isInTree(dict, "testWord") == true);
    assert(isInTree(dict, "otherTest") == false);

    recover = getValue(dict, "testWord");
    assert(recover != someTestObject); // the adress should have changed
    assert(recover == someOtherTestObject); // now it should be this one

    removeValue(&dict, "testWord"); //remove the node from the tree
    assert(isInTree(dict, "testWord") == false);
    assert(isInTree(dict, "otherTest") == false);

    insertValue(&dict, makeKey("testWord"), (void*)someTestObject, false);
    insertValue(&dict, makeKey("otherTest"), (void*)someOtherTestObject, false);

    assert(isInTree(dict, "testWord") == true);
    assert(isInTree(dict, "otherTest") == true);
    freeTreeMap(&dict); // should free both
    assert(isInTree(dict, "testWord") == false);
    assert(isInTree(dict, "otherTest") == false);

    printf("----- TEST SUCCESS -----\n\n");
}

/**
 * @brief this function is there to generate fake key for testing
 * @param k some unsigned int
 * @return a valid key to use in the TreeMap
 */
const char* fakeKey(unsigned int k){
    // seed \in [32, 122]
    unsigned int seed = (k % 91) + 32;
    unsigned int count = k / 91;
    count += 2;

    char* key = (char*)malloc(sizeof(char) * count);
    for(unsigned int i = 0; i < count-1; i++){
        key[i] = (char)seed; // make a printable ascii character

        // get another seed
        seed = ((seed ^ count >> i) % 91) + 32;
    }

    // add the null terminator
    key[count-1] = '\0';

    return key;
}

void testTreeManyInsert(int insertCount){
    printf("----- TEST MANY INSERT -----\n");

    TreeMap* dict = initTreeMap();
    printMallocStat();

    // add a lot of stuff
    for(int i = 45; i < insertCount; i++){
        testObject* someTestElement = (testObject*)malloc(sizeof(testObject));
        someTestElement->evenAPoiter = NULL;
        someTestElement->oneBool = true;
        someTestElement->someDouble = 3.14 * i;
        someTestElement->someInt = i;

        insertValue(&dict, fakeKey(i), (void*)someTestElement, true);
    }

    printMallocStat();

    // delete half of it
    for(int i = insertCount/2; i < insertCount; i++){
        const char* key = fakeKey(i);
        deleteValue(&dict, key);
        free((char*) key);
    }
    printMallocStat();

    // insert a new half
    for(int i = 0; i < insertCount/2; i++){
        int k = insertCount + i;
        testObject* someTestElement = (testObject*)malloc(sizeof(testObject));
        someTestElement->evenAPoiter = NULL;
        someTestElement->oneBool = true;
        someTestElement->someDouble = 3.14 * i;
        someTestElement->someInt = i;

        insertValue(&dict, fakeKey(k), (void*)someTestElement, true);
    }
    printMallocStat();

    // we free the whole thing
    freeTreeMap(&dict);

    printMallocStat();

    printf("----- TEST SUCCESS -----\n\n");
}
