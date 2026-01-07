/**
 * @author Arthus Doriath
 * @date 01/2026
 */

#ifndef TREE_MAP_H
#define TREE_MAP_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

/**
 * @brief This is a RedBlack tree over string keys which may contain any type.
 * This behave just like a dictionary over string key.
 * since the content are void* it is recommended that you precise the type in the name
 */
typedef struct TreeMap_t{
    struct TreeMap_t* left;
    struct TreeMap_t* right;

    void* content;
    const char* key;
} TreeMap;

/**
 * 
 */
TreeMap* initTreeMap(void);

/**
 * @warning this function free all pointers that are still withing the tree, therefore all related pointers are invalidated
 * @note after this opperation, the mapp pointer will be NULL and will need to be initialized again
 */
void freeTreeMap(TreeMap** mapp);

/**
 * @brief this is a function to turn a stack allocated text into a heap allocated text that can be used as a key for the tree
 * if the text has been allocated with malloc or similar then this function isn't necessary
 * 
 * @param stackText a null terminated text allocated on the stack
 * @return a pointer to a null terminated string to be used as key in the tree
 * 
 * @warning if the text is not null terminated this will cause undefined behavior
 */
const char* makeKey(const char* stackText);

/**
 * @note in case the key already exist, the value will be replaced but no free will be called, please make sure the value is freed somewhere
 * @warning this function assume that key is a null terminated string, undefined behaviors WILL happen if it is not
 */
void insertValue(TreeMap** map, const char* key, void* value, bool freeOnOverride);

/**
 * @note complexity is estimated to be O(n log(n)) with n being the depth of the tree
 */
void* getValue(const TreeMap* map, const char* key);

/**
 * @brief takes the tree in src and insert each nodes in dest
 * @param src
 * @param dest
 * @return ...
 *
 * @post dest take ownership of the data in src
 * @warning src must not be freed since dest is now the owner
 * @note this function is mostly for internal uses, be sure of what you're doing
 */
TreeMap* reinsert(TreeMap* src, TreeMap* dest);

/**
 * 
 */
void removeValue(TreeMap** mapp, const char* key);

/**
 * 
 */
void deleteValue(TreeMap** map, const char* key);

/**
 * @brief takes the value out of the tree
 * @param mapp a double pointer to the tree
 * @param key a valid null terminated key allocated on the heap
 * @return a void* to the content of that node, the pointer may be NULL if no data was in the tree
 *
 * @warning the user takes ownership of the data and is expected to free it
 */
void* popValue(TreeMap** mapp, const char* key);

/**
 * 
 */
bool isInTree(const TreeMap* map, const char* key);

/**
 * 
 */
void rebalanceTree(TreeMap** map);

/**
 * 
 */
bool needRebalancing(const TreeMap* map);

#endif
