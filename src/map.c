#include "map.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

void map_init(Map* map) {
    memset(map, 0, sizeof(Map));
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            map->tiles[x][y].type = TILE_WALL;
        }
    }
}

void map_set_tile(Map* map, int x, int y, char type) {
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) return;
    map->tiles[x][y].type = type;
    unsigned char flags = 0;
    if (type == TILE_FLOOR || type == TILE_STAIRS_DOWN) {
        flags |= TILE_FLAG_WALKABLE | TILE_FLAG_TRANSPARENT;
    } else if (type == TILE_DOOR) {
        flags |= TILE_FLAG_WALKABLE | TILE_FLAG_TRANSPARENT;
    }
    map->tiles[x][y].flags = flags;
}

Tile* map_get_tile(Map* map, int x, int y) {
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) return NULL;
    return &map->tiles[x][y];
}

int is_walkable(Map* map, int x, int y) {
    Tile* t = map_get_tile(map, x, y);
    if (!t) return 0;
    return (t->flags & TILE_FLAG_WALKABLE) != 0;
}

int is_transparent(Map* map, int x, int y) {
    Tile* t = map_get_tile(map, x, y);
    if (!t) return 0;
    return (t->flags & TILE_FLAG_TRANSPARENT) != 0;
}

typedef struct {
    int x, y, w, h;
} Room;

static int room_overlap(Room* a, Room* b) {
    return !(a->x + a->w < b->x || b->x + b->w < a->x ||
             a->y + a->h < b->y || b->y + b->h < a->y);
}

static void carve_room(Map* map, Room* r) {
    for (int y = r->y; y < r->y + r->h; y++) {
        for (int x = r->x; x < r->x + r->w; x++) {
            map_set_tile(map, x, y, TILE_FLOOR);
        }
    }
}

static void carve_h_corridor(Map* map, int x1, int x2, int y) {
    for (int x = (x1 < x2 ? x1 : x2); x <= (x1 < x2 ? x2 : x1); x++) {
        map_set_tile(map, x, y, TILE_FLOOR);
    }
}

static void carve_v_corridor(Map* map, int y1, int y2, int x) {
    for (int y = (y1 < y2 ? y1 : y2); y <= (y1 < y2 ? y2 : y1); y++) {
        map_set_tile(map, x, y, TILE_FLOOR);
    }
}

void map_generate(Map* map) {
    map_init(map);

    Room rooms[12];
    int num_rooms = 0;
    int max_rooms = 10 + (rand() % 3);

    for (int i = 0; i < max_rooms; i++) {
        Room r;
        r.w = 4 + (rand() % 6);
        r.h = 3 + (rand() % 5);
        r.x = 1 + (rand() % (MAP_WIDTH - r.w - 2));
        r.y = 1 + (rand() % (MAP_HEIGHT - r.h - 2));

        int overlap = 0;
        for (int j = 0; j < num_rooms; j++) {
            if (room_overlap(&r, &rooms[j])) {
                overlap = 1;
                break;
            }
        }

        if (!overlap) {
            carve_room(map, &r);
            if (num_rooms > 0) {
                Room* prev = &rooms[num_rooms - 1];
                int px = prev->x + prev->w / 2;
                int py = prev->y + prev->h / 2;
                int cx = r.x + r.w / 2;
                int cy = r.y + r.h / 2;
                if (rand() % 2) {
                    carve_h_corridor(map, px, cx, py);
                    carve_v_corridor(map, py, cy, cx);
                } else {
                    carve_v_corridor(map, py, cy, px);
                    carve_h_corridor(map, px, cx, cy);
                }
            }
            rooms[num_rooms++] = r;
        }
    }

    // Place stairs in last room
    if (num_rooms > 0) {
        Room* last = &rooms[num_rooms - 1];
        map_set_tile(map, last->x + last->w/2, last->y + last->h/2, TILE_STAIRS_DOWN);
    }
}

// Recursive shadowcasting FOV
static void cast_light(Map* map, int cx, int cy, int row,
                       float start_slope, float end_slope, int radius,
                       int xx, int xy, int yx, int yy) {
    if (start_slope < end_slope) return;
    float next_start_slope = start_slope;
    for (int i = row; i <= radius; i++) {
        int blocked = 0;
        for (int dx = -i, dy = -i; dx <= 0; dx++) {
            float l_slope = (dx - 0.5) / (dy + 0.5);
            float r_slope = (dx + 0.5) / (dy - 0.5);
            if (start_slope < r_slope) continue;
            else if (end_slope > l_slope) break;

            int sax = dx * xx + dy * xy;
            int say = dx * yx + dy * yy;
            if ((sax < 0 && abs(sax) > cx) || (say < 0 && abs(say) > cy)) continue;

            int ax = cx + sax;
            int ay = cy + say;
            if (ax < 0 || ax >= MAP_WIDTH || ay < 0 || ay >= MAP_HEIGHT) continue;

            int dist = (int)sqrt(dx*dx + dy*dy);
            if (dist <= radius) {
                map->tiles[ax][ay].visible = 1;
                map->tiles[ax][ay].flags |= TILE_FLAG_EXPLORED;
            }

            if (blocked) {
                if (!is_transparent(map, ax, ay)) {
                    next_start_slope = r_slope;
                } else {
                    blocked = 0;
                    start_slope = next_start_slope;
                }
            } else if (!is_transparent(map, ax, ay) && i < radius) {
                blocked = 1;
                next_start_slope = r_slope;
                cast_light(map, cx, cy, i + 1, start_slope, l_slope, radius,
                           xx, xy, yx, yy);
            }
        }
        if (blocked) break;
    }
}

void compute_fov(Map* map, int x, int y, int radius) {
    for (int j = 0; j < MAP_HEIGHT; j++) {
        for (int i = 0; i < MAP_WIDTH; i++) {
            map->tiles[i][j].visible = 0;
        }
    }
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) return;
    map->tiles[x][y].visible = 1;
    map->tiles[x][y].flags |= TILE_FLAG_EXPLORED;

    int mult[4][4] = {
        {1, 0, 0, 1},
        {0, 1, 1, 0},
        {0, -1, 1, 0},
        {1, 0, 0, -1}
    };
    for (int i = 0; i < 4; i++) {
        cast_light(map, x, y, 1, 1.0, 0.0, radius,
                   mult[i][0], mult[i][1], mult[i][2], mult[i][3]);
    }
}
