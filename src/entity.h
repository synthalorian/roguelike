#ifndef ENTITY_H
#define ENTITY_H

#define MAX_ENTITIES 64

typedef enum {
    ENTITY_PLAYER,
    ENTITY_GOBLIN,
    ENTITY_ORC,
    ENTITY_ITEM_POTION
} EntityType;

typedef struct {
    int id;
    int active;
    int x, y;
    int hp, max_hp;
    int attack;
    int defense;
    EntityType type;
    char symbol;
    int color_pair;
} Entity;

typedef struct {
    Entity entities[MAX_ENTITIES];
    int count;
} EntityManager;

void entity_manager_init(EntityManager* em);
Entity* entity_create(EntityManager* em, EntityType type, int x, int y);
void entity_destroy(EntityManager* em, int id);
Entity* entity_at(EntityManager* em, int x, int y);

void entity_move(Entity* e, int dx, int dy);
void entity_take_damage(Entity* e, int dmg);

#endif
