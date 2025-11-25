#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

/**
 * @brief read a line from the standard input, this function will read at most maxlen-1 characters
 * and add a null terminator at the end
 * @param out where to write the line that got read
 * @param maxlen the maximal size of the buffer
 * @return how many character actually got read (does not count the null terminator)
 */
size_t readln(char* out, size_t maxlen);

/**
 * @brief takes a null terminated string and will split it into token according to the delimiter
 * each token is null terminated and will need to be freed after usage
 * @param source the original string we read
 * @param delimiter the element we use to tell the end of one token and the start of another
 * @return an array of strings representing each separated token
 * 
 * or NULL if any error occured
 */
/*
char** split(const char* source, const char* delimiter);
*/

#endif