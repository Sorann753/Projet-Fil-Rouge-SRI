#ifndef __TOKEN_H__
#define __TOKEN_H__

/**
 * TODO: definie le token (mot)
 */

#include <string.h>
#include "./command.h"

/*tokentype != action_t*/
typedef enum tokentype
{
    TOK_VERBE,
    TOK_NUM,
    TOK_COLOR,
    TOK_OBJECT,
    TOK_UNKNOWN,
    TOK_NONE
} tokentype;

/**
 * @brief token[n] = {texte, type}
 */
typedef struct token
{
    char texte[100];
    tokentype type;

    /*union: permet davoir une parmis les value suivante*/
    union
    {
        action_t action;       // pour TOK_VERBE
        float value;           // pour TOK_NUM
        color_t color;         // pour TOK_COLOR
        object_t object;       // pour TOK_OBJECT
        direction_t direction; // pour la direction
    } data;
} token;

#endif