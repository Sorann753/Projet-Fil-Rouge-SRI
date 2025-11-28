#include "utils/stringUtils.h"

/**
 * @brief lire les ligne a partir du teminal (alternative au scanf)
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