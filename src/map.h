#ifndef MAP_H
#define MAP_H

#define MAP_WIDTH 80
#define MAP_HEIGHT 24

typedef enum {
    TILE_WALL = '#',
    TILE_FLOOR = '.',
    TILE_STAIRS_DOWN = '>',
    TILE_DOOR = '+'
} TileType;

// Tile flags
#define TILE_FLAG_WALKABLE  0x01
#define TILE_FLAG_TRANSPARENT 0x02
#define TILE_FLAG_EXPLORED  0x04

typedef struct {
    char type;
    unsigned char flags;
    unsigned char visible;  // currently in FOV
} Tile;

typedef struct {
    Tile tiles[MAP_WIDTH][MAP_HEIGHT];
} Map;

void map_init(Map* map);
void map_generate(Map* map);
void map_set_tile(Map* map, int x, int y, char type);
Tile* map_get_tile(Map* map, int x, int y);

int is_walkable(Map* map, int x, int y);
int is_transparent(Map* map, int x, int y);

void compute_fov(Map* map, int x, int y, int radius);

#endif
