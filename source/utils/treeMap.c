/**
 * @author Arthus Doriath
 * @date 01/2026
 */

#include "utils/treeMap.h"

TreeMap* initTreeMap(void){
    TreeMap* T = (TreeMap*)malloc(sizeof(TreeMap));
    T->content = NULL;
    T->key = NULL;
    T->left = NULL;
    T->right = NULL;

    return T;
}

void freeTreeMap(TreeMap** mapp){
    if(mapp == NULL) return;

    //aliasing to make it easy to write
    TreeMap* map = *mapp;
    if(map == NULL) return;

    // clean key and content
    free(map->content);
    free((char*)map->key);
    
    // secure the pointers
    map->content = NULL;
    map->key = NULL;

    // delete the other branches
    freeTreeMap(&map->left);
    freeTreeMap(&map->right);
    map->left = NULL;
    map->right = NULL;

    // now that everything down the line is clean
    // we free ourselves
    free(map);
    // we NULL both pointers to the expired data
    map = NULL;
    *mapp = NULL;
}

const char* makeKey(const char* stackText){
    if(stackText == NULL) return NULL;

    size_t length = strlen(stackText) + 1;

    // allocate the buffer (we sizeof here just by paranoia but char is supposedly 1 byte)
    char* buffer = (char*)malloc(sizeof(char) * length);

    strncpy(buffer, stackText, length);
    
    return buffer;
}

void insertValue(TreeMap** mapp, const char* HeapKey, void* value, bool freeOnOverride){
    assert(mapp != NULL);
    assert(HeapKey != NULL);

    TreeMap* map = *mapp; // alias for easy access
    if(map == NULL){
        *mapp = initTreeMap();
        map = *mapp;
    }

    if(map->key == NULL){
        map->key = HeapKey;
        map->content = value;
        return;
    }

    int order = strcmp(HeapKey, map->key);
    if(order < 0){ // value goes on the left
        insertValue(&(map->left), HeapKey, value, freeOnOverride);
    }
    else if(order > 0){ // value goes on the right
        insertValue(&(map->right), HeapKey, value, freeOnOverride);
    }
    else{ // this is the same key, so we just change the value
        //! MAKE SURE THAT THE VALUE IS FREED SOMEWHERE
        if(freeOnOverride){
            free(map->content);
        }
        map->content = value;
        // free the new key since it was already there and not needed
        free((char*)HeapKey);
    }

    if(needRebalancing(map)){ 
        rebalanceTree(mapp);
    }
}

void* getValue(const TreeMap* map, const char* key){
    if(map == NULL || key == NULL) return NULL;
    if(map->key == NULL) return NULL;

    int order = strcmp(key, map->key);
    if(order < 0){ // the value is on the left
        if(map->left == NULL){
            // we're at the end of the tree but we didn't find anything
            return NULL;
        }
        else{ // we keep searching
            return getValue(map->left, key);
        }
    }
    else if(order > 0){ // the value is on the right
        if(map->right == NULL){
            // we're at the end of the tree but we didn't find anything
            return NULL;
        }
        else{
            return getValue(map->right, key);
        }
    }
    else{ // we found the value
        return map->content;
    }
}

TreeMap* reinsert(TreeMap* src, TreeMap* dest){
    if(src == NULL) return dest; // nothing to reinsert
    if(dest == NULL) return src; // does nothing
    /** @post src != NULL && dest != NULL */

    // handle left and right branch
    dest = reinsert(src->left, dest);
    dest = reinsert(src->right, dest);
    /** @post dest has ownership of all data in the left and right branch */

    // handle the value on the current node
    insertValue(&dest, src->key, src->content, false);

    return dest;
}

// remove without freeing the content, however we do free the key 
//? note : maybe it would be better to create the key when we
//? insert the value and just not let the user have a copy of that pointer
void removeValue(TreeMap** mapp, const char* key){
    if(mapp == NULL) return;
    if(key == NULL) return;

    // alias to simplify the code
    TreeMap* map = *mapp;

    if(map == NULL) return;
    if(map->key == NULL) return;

    int order = strcmp(key, map->key);
    if(order < 0){
        // the data to remove is on the left
        removeValue(&map->left, key);
    }
    else if(order > 0){
        // the data to remove is on the right
        removeValue(&map->right, key);
    }
    else{ // we found the data
        free((char*)map->key);
        map->key = NULL;

        //! we explicitly DO NOT free the content pointer
        //! this function assume the user will do it themselves and has a copy
        map->content = NULL;

        // we're replacing the current node with one of the child nodes
        // if no childs then NULL
        TreeMap* newNode = reinsert(map->left, map->right);
        free(map); // we free the old containing node
        map = NULL;
        *mapp = newNode;
    }
}

void deleteValue(TreeMap** mapp, const char* key){
    if(mapp == NULL) return;
    if(key == NULL) return;

    // alias to simplify the code
    TreeMap* map = *mapp;

    if(map == NULL) return;
    if(map->key == NULL) return;

    int order = strcmp(key, map->key);
    if(order < 0){
        // the data to remove is on the left
        deleteValue(&(map->left), key);
    }
    else if(order > 0){
        // the data to remove is on the right
        deleteValue(&(map->right), key);
    }
    else{ // we found the data
        free((char*)map->key);
        map->key = NULL;

        free(map->content);
        map->content = NULL;

        // we're replacing the current node with one of the child nodes
        // if no childs then NULL
        TreeMap* newNode = reinsert(map->left, map->right);
        free(map); // we free the old containing node
        map = NULL;
        *mapp = newNode;
    }
}

/**
 * @brief takes the value out of the tree
 * @param mapp a double pointer to the tree
 * @param key a valid null terminated key allocated on the heap
 * @return a void* to the content of that node, the pointer may be NULL if no data was in the tree
 *
 * @warning the user takes ownership of the data and is expected to free it
 */
void* popValue(TreeMap** mapp, const char* key){
    void* content = getValue(*mapp, key);
    removeValue(mapp, key);

    return content;
}

bool isInTree(const TreeMap* map, const char* key){
    if(map == NULL || key == NULL) return false;
    if(map->key == NULL) return false;

    int order = strcmp(key, map->key);
    if(order < 0){ // the value is on the left
        if(map->left == NULL){
            // we're at the end of the tree but we didn't find anything
            return false;
        }
        else{ // we keep searching
            return isInTree(map->left, key);
        }
    }
    else if(order > 0){ // the value is on the right
        if(map->right == NULL){
            // we're at the end of the tree but we didn't find anything
            return false;
        }
        else{
            return isInTree(map->right, key);
        }
    }
    else{ // we found the value
        return true;
    }
}

// NOTE : those are functions to improve the tree but they are
// not necessary for it to work
void rebalanceTree(TreeMap** map){
    //TODO
}

bool needRebalancing(const TreeMap* map){
    return false; //TODO
}