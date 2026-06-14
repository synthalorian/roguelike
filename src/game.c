#include "game.h"
#include "map.h"
#include "entity.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int find_empty_floor(Map* map) {
    int attempts = 0;
    while (attempts < 1000) {
        int x = 1 + rand() % (MAP_WIDTH - 2);
        int y = 1 + rand() % (MAP_HEIGHT - 2);
        if (is_walkable(map, x, y)) {
            return x + y * MAP_WIDTH;
        }
        attempts++;
    }
    return -1;
}

void game_init(Game* g) {
    memset(g, 0, sizeof(Game));
    g->state = STATE_PLAYING;
    g->floor_level = 1;
    g->score = 0;
    g->turn = 0;
    strcpy(g->message, "Welcome to the dungeon. Find the stairs.");
    game_new_floor(g);
}

void game_new_floor(Game* g) {
    entity_manager_init(&g->entities);
    map_generate(&g->map);
    
    int pos = find_empty_floor(&g->map);
    if (pos >= 0) {
        int x = pos % MAP_WIDTH;
        int y = pos / MAP_WIDTH;
        g->player = entity_create(&g->entities, ENTITY_PLAYER, x, y);
    }
    
    int enemy_count = 3 + (g->floor_level * 2);
    game_spawn_enemies(g, enemy_count);
    
    compute_fov(&g->map, g->player->x, g->player->y, 8);
    sprintf(g->message, "Floor %d. %d enemies lurk in the dark.", g->floor_level, enemy_count);
}

void game_spawn_enemies(Game* g, int count) {
    for (int i = 0; i < count; i++) {
        int pos = find_empty_floor(&g->map);
        if (pos < 0) continue;
        int x = pos % MAP_WIDTH;
        int y = pos / MAP_WIDTH;
        
        // Don't spawn too close to player
        int dx = abs(x - g->player->x);
        int dy = abs(y - g->player->y);
        if (dx < 5 && dy < 5) continue;
        
        EntityType type = (rand() % 3 == 0) ? ENTITY_ORC : ENTITY_GOBLIN;
        entity_create(&g->entities, type, x, y);
    }
}

void game_process_turn(Game* g, int dx, int dy) {
    if (g->state != STATE_PLAYING) return;
    
    int tx = g->player->x + dx;
    int ty = g->player->y + dy;
    
    if (tx < 0 || tx >= MAP_WIDTH || ty < 0 || ty >= MAP_HEIGHT) return;
    
    Tile* tile = map_get_tile(&g->map, tx, ty);
    if (!tile) return;
    
    // Check for enemy at target
    Entity* target = entity_at(&g->entities, tx, ty);
    if (target && target->active && target->type != ENTITY_PLAYER) {
        entity_take_damage(target, g->player->attack);
        if (target->active) {
            sprintf(g->message, "You hit the %s for %d dmg! (HP: %d)", 
                    target->symbol == 'g' ? "goblin" : "orc",
                    g->player->attack - target->defense, target->hp);
        } else {
            sprintf(g->message, "You killed the %s!",
                    target->symbol == 'g' ? "goblin" : "orc");
            g->score += (target->type == ENTITY_ORC) ? 20 : 10;
        }
    } else if (is_walkable(&g->map, tx, ty)) {
        g->player->x = tx;
        g->player->y = ty;
        
        // Check stairs
        if (tile->type == TILE_STAIRS_DOWN) {
            g->floor_level++;
            g->score += 50;
            game_new_floor(g);
            return;
        }
    } else {
        return; // bump into wall
    }
    
    g->turn++;
    game_enemy_turn(g);
    compute_fov(&g->map, g->player->x, g->player->y, 8);
    
    if (g->player->hp <= 0) {
        g->state = STATE_GAME_OVER;
        strcpy(g->message, "You died. Game over.");
    }
}

static void enemy_ai(Entity* e, Game* g) {
    if (!e->active) return;
    
    int dx = 0, dy = 0;
    int dist_x = abs(e->x - g->player->x);
    int dist_y = abs(e->y - g->player->y);
    
    // Simple chase: if in FOV, move toward player
    Tile* tile = map_get_tile(&g->map, e->x, e->y);
    if (tile && tile->visible) {
        if (dist_x > dist_y) {
            dx = (e->x < g->player->x) ? 1 : -1;
        } else {
            dy = (e->y < g->player->y) ? 1 : -1;
        }
    } else {
        // Random wander
        dx = (rand() % 3) - 1;
        dy = (rand() % 3) - 1;
    }
    
    int tx = e->x + dx;
    int ty = e->y + dy;
    
    // Attack player if adjacent
    if (tx == g->player->x && ty == g->player->y) {
        entity_take_damage(g->player, e->attack);
        sprintf(g->message, "The %s hits you for %d dmg! (HP: %d)",
                e->symbol == 'g' ? "goblin" : "orc",
                e->attack - g->player->defense, g->player->hp);
        return;
    }
    
    if (is_walkable(&g->map, tx, ty) && !entity_at(&g->entities, tx, ty)) {
        e->x = tx;
        e->y = ty;
    }
}

void game_enemy_turn(Game* g) {
    for (int i = 0; i < g->entities.count; i++) {
        Entity* e = &g->entities.entities[i];
        if (e->active && e->type != ENTITY_PLAYER) {
            enemy_ai(e, g);
        }
    }
}
