#include "input.h"
#include "game.h"
#include <ncurses.h>

InputResult get_input() {
    int ch = getch();
    
    switch (ch) {
        case 'h': case KEY_LEFT:  return INPUT_MOVE_W;
        case 'j': case KEY_DOWN:  return INPUT_MOVE_S;
        case 'k': case KEY_UP:    return INPUT_MOVE_N;
        case 'l': case KEY_RIGHT: return INPUT_MOVE_E;
        case 'y': return INPUT_MOVE_NW;
        case 'u': return INPUT_MOVE_NE;
        case 'b': return INPUT_MOVE_SW;
        case 'n': return INPUT_MOVE_SE;
        case '>': return INPUT_STAIRS;
        case 'q': case 'Q': return INPUT_QUIT;
        case 'r': case 'R': return INPUT_NEW_GAME;
        default: return INPUT_NONE;
    }
}
