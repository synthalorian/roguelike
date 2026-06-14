#include "entity.h"
#include <string.h>

void entity_manager_init(EntityManager* em) {
    memset(em, 0, sizeof(EntityManager));
    em->count = 0;
}

Entity* entity_create(EntityManager* em, EntityType type, int x, int y) {
    if (em->count >= MAX_ENTITIES) return NULL;
    Entity* e = &em->entities[em->count];
    e->id = em->count;
    e->active = 1;
    e->x = x;
    e->y = y;
    e->type = type;
    e->color_pair = 1;

    switch (type) {
        case ENTITY_PLAYER:
            e->symbol = '@';
            e->hp = 30; e->max_hp = 30;
            e->attack = 5; e->defense = 2;
            e->color_pair = 2;
            break;
        case ENTITY_GOBLIN:
            e->symbol = 'g';
            e->hp = 10; e->max_hp = 10;
            e->attack = 3; e->defense = 0;
            e->color_pair = 3;
            break;
        case ENTITY_ORC:
            e->symbol = 'o';
            e->hp = 20; e->max_hp = 20;
            e->attack = 5; e->defense = 1;
            e->color_pair = 4;
            break;
        case ENTITY_ITEM_POTION:
            e->symbol = '!';
            e->hp = 0; e->max_hp = 0;
            e->attack = 0; e->defense = 0;
            e->color_pair = 5;
            break;
    }
    em->count++;
    return e;
}

void entity_destroy(EntityManager* em, int id) {
    if (id < 0 || id >= em->count) return;
    em->entities[id].active = 0;
}

Entity* entity_at(EntityManager* em, int x, int y) {
    for (int i = 0; i < em->count; i++) {
        if (em->entities[i].active && em->entities[i].x == x && em->entities[i].y == y) {
            return &em->entities[i];
        }
    }
    return NULL;
}

void entity_move(Entity* e, int dx, int dy) {
    e->x += dx;
    e->y += dy;
}

void entity_take_damage(Entity* e, int dmg) {
    int actual = dmg - e->defense;
    if (actual < 1) actual = 1;
    e->hp -= actual;
    if (e->hp <= 0) {
        e->hp = 0;
        e->active = 0;
    }
}
