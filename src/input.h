#ifndef INPUT_H
#define INPUT_H

typedef enum {
    INPUT_NONE,
    INPUT_MOVE_N,
    INPUT_MOVE_S,
    INPUT_MOVE_E,
    INPUT_MOVE_W,
    INPUT_MOVE_NE,
    INPUT_MOVE_NW,
    INPUT_MOVE_SE,
    INPUT_MOVE_SW,
    INPUT_STAIRS,
    INPUT_QUIT,
    INPUT_NEW_GAME
} InputResult;

InputResult get_input(void);

#endif
