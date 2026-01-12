#ifndef __COMMAND_H__
#define __COMMAND_H__

/*le maximum de commande dans command_list*/
#define MAX_COMMAND 20

/**
 * TODO: ajout des unitées
 */

typedef enum action_t
{
    ACT_FORWARD,
    ACT_BACKWARD,
    ACT_TURN,
    ACT_STOP,
    ACT_SEARCH, // (Vision)
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
 * @brief commande[n] = {action, value, color, object, direction, unit}
 */
typedef struct command
{
    action_t action;
    float value;
    // TODO: ajouter la conidtion valeur null
    color_t color;
    object_t object;
    direction_t direction;

    int negated; // =1 si "Ne pas" sinon =0
} command;

/**
 * @brief liste de command
 */
typedef struct command_list
{
    command cmd[MAX_COMMAND];
    int count;

} command_list;

#endif