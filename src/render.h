#ifndef RENDER_H
#define RENDER_H

#include "game.h"

void render_init(void);
void render_shutdown(void);
void render_game(Game* g);
void render_game_over(Game* g);

#endif
