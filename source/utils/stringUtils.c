#include "utils/stringUtils.h"

/**
 * @brief read a line from the standard input, this function will read at most maxlen-1 characters
 * and add a null terminator at the end
 * @param out where to write the line that got read
 * @param maxlen the maximal size of the buffer
 * @return how many character actually got read (does not count the null terminator)
 */
size_t readln(char* out, const size_t maxlen){
    assert(out != NULL);
    char in;
    bool hasText = true;
    
    size_t readCount = 0;
    while(hasText){
        in = getchar();
        if(in == '\n' || in == EOF){ // end of line or end of reading
            hasText = false;
        }
        else if(readCount < maxlen - 1){
            out[readCount] = in;
            readCount++;
        }
        else{
            // the remaining char are just consumed to flush the buffer between reads
        }
    }

    out[readCount] = '\0';
    return readCount;
}



/**
 * @brief takes a null terminated string and will split it into token according to the delimiter
 * each token is null terminated and will need to be freed after usage
 * @param source the original string we read
 * @param delimiter the element we use to tell the end of one token and the start of another
 * @param delimiterLength how many characters are in the delimiter (not counting the null terminator)
 * @return an array of strings representing each separated token, the last element point to NULL
 * 
 * or NULL if any error occured
 */
/** @details  this function got cancelled since it may have been overkill for the project
char** split(const char* source, const char* delimiter, size_t delimiterLength){
    assert(source != NULL);
    assert(delimiter != NULL);

    char* buffer[255];
    bool cleanupNeeded = false;
    size_t tokenCount = 0;

    const char* current = source;
    const char* next;
    while((next = strstr(current, delimiter)) != NULL){ // while the assigned value is not NULL
        size_t tokenLength = next - current;

        buffer[tokenCount] = (char*)malloc(sizeof(char) * (tokenLength+1));

        strncpy(buffer[tokenCount], current, tokenLength);
        buffer[tokenCount][tokenLength] = '\0';

        current = next + delimiterLength;
        tokenCount++;

        if(tokenCount >= 255){ // buffer is full
            //
        }
    }

    if(cleanupNeeded){
        // some error happened so we need to free everything before we return NULL
    }
}
*/