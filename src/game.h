#ifndef GAME_H
#define GAME_H

#include "map.h"
#include "entity.h"

typedef enum {
    STATE_PLAYING,
    STATE_GAME_OVER,
    STATE_VICTORY
} GameState;

typedef struct {
    Map map;
    EntityManager entities;
    Entity* player;
    GameState state;
    int floor_level;
    int score;
    int turn;
    char message[128];
} Game;

void game_init(Game* g);
void game_new_floor(Game* g);
void game_process_turn(Game* g, int dx, int dy);
void game_enemy_turn(Game* g);
void game_spawn_enemies(Game* g, int count);

#endif
