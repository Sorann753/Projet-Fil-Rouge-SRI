#ifndef __COMMAND_H__
#define __COMMAND_H__

#define MAX_COMMAND 20

typedef enum action_t
{
    ACT_FORWARD,
    ACT_BACKWARD,
    ACT_TURN,
    ACT_STOP,
    ACT_SEARCH,
    ACT_NONE
} action_t;

typedef enum direction_t
{
    DIR_RIGHT,
    DIR_LEFT,
    DIR_NONE
} direction_t;

typedef enum color_t
{
    COL_BLUE,
    COL_RED,
    COL_ORANGE,
    COL_YELLOW,
    COL_BLACK,
    COL_WHITE,
    COL_GREEN,
    COL_NONE
} color_t;

typedef enum object_t
{
    OBJ_CUBE,
    OBJ_BALL,
    OBJ_NONE
} object_t;

/**
 * @brief Une commande = action + paramètres
 */
typedef struct command
{
    action_t action;
    float value;
    color_t color;
    object_t object;
    direction_t direction;
    int negated;
} command;

/**
 * @brief Liste de commandes
 */
typedef struct command_list
{
    command cmd[MAX_COMMAND]; /* ← Tableau de structures, PAS de pointeurs */
    int count;
} command_list;

#endif