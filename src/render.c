#include "render.h"
#include "game.h"
#include "map.h"
#include "entity.h"
#include <ncurses.h>
#include <string.h>

static int colors_initialized = 0;

void render_init() {
    initscr();
    raw();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_WHITE, COLOR_BLACK);
        init_pair(2, COLOR_YELLOW, COLOR_BLACK);   // player
        init_pair(3, COLOR_GREEN, COLOR_BLACK);    // goblin
        init_pair(4, COLOR_RED, COLOR_BLACK);      // orc
        init_pair(5, COLOR_MAGENTA, COLOR_BLACK);  // item
        init_pair(6, COLOR_CYAN, COLOR_BLACK);     // stairs
        colors_initialized = 1;
    }
}

void render_shutdown() {
    endwin();
}

void render_game(Game* g) {
    clear();
    
    // Draw map
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            Tile* tile = map_get_tile(&g->map, x, y);
            if (!tile) continue;
            
            if (tile->visible) {
                attron(COLOR_PAIR(1));
                if (tile->type == TILE_STAIRS_DOWN) {
                    attron(COLOR_PAIR(6));
                    mvaddch(y, x, tile->type);
                    attroff(COLOR_PAIR(6));
                } else {
                    mvaddch(y, x, tile->type);
                }
                attroff(COLOR_PAIR(1));
            } else if (tile->flags & TILE_FLAG_EXPLORED) {
                attron(COLOR_PAIR(1));
                attron(A_DIM);
                mvaddch(y, x, tile->type);
                attroff(A_DIM);
                attroff(COLOR_PAIR(1));
            }
        }
    }
    
    // Draw entities
    for (int i = 0; i < g->entities.count; i++) {
        Entity* e = &g->entities.entities[i];
        if (!e->active) continue;
        
        Tile* tile = map_get_tile(&g->map, e->x, e->y);
        if (tile && tile->visible) {
            attron(COLOR_PAIR(e->color_pair));
            mvaddch(e->y, e->x, e->symbol);
            attroff(COLOR_PAIR(e->color_pair));
        }
    }
    
    // Draw status bar at bottom
    int status_y = MAP_HEIGHT + 1;
    attron(COLOR_PAIR(1));
    mvprintw(status_y, 0, "Floor: %d  HP: %d/%d  ATK: %d  DEF: %d  Score: %d  Turn: %d",
             g->floor_level, g->player->hp, g->player->max_hp,
             g->player->attack, g->player->defense, g->score, g->turn);
    attroff(COLOR_PAIR(1));
    
    // Draw message
    attron(COLOR_PAIR(1));
    mvprintw(status_y + 1, 0, "%-*s", MAP_WIDTH, g->message);
    attroff(COLOR_PAIR(1));
    
    // Help line
    attron(COLOR_PAIR(1));
    attron(A_DIM);
    mvprintw(status_y + 2, 0, "h/j/k/l=move  > =stairs  r=restart  Q=quit");
    attroff(A_DIM);
    attroff(COLOR_PAIR(1));
    
    refresh();
}

void render_game_over(Game* g) {
    clear();
    attron(COLOR_PAIR(4) | A_BOLD);
    mvprintw(10, 30, "YOU DIED");
    attroff(COLOR_PAIR(4) | A_BOLD);
    
    attron(COLOR_PAIR(1));
    mvprintw(12, 25, "Floor: %d  Score: %d  Turns: %d", g->floor_level, g->score, g->turn);
    mvprintw(14, 25, "Press 'r' for new run, 'q' to quit");
    attroff(COLOR_PAIR(1));
    
    refresh();
}
