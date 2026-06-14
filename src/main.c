#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "game.h"
#include "render.h"
#include "input.h"

int main() {
    srand(time(NULL));
    
    Game game;
    game_init(&game);
    
    render_init();
    
    int running = 1;
    while (running) {
        if (game.state == STATE_PLAYING) {
            render_game(&game);
        } else if (game.state == STATE_GAME_OVER) {
            render_game_over(&game);
        }
        
        InputResult input = get_input();
        
        switch (input) {
            case INPUT_MOVE_N:
                game_process_turn(&game, 0, -1);
                break;
            case INPUT_MOVE_S:
                game_process_turn(&game, 0, 1);
                break;
            case INPUT_MOVE_E:
                game_process_turn(&game, 1, 0);
                break;
            case INPUT_MOVE_W:
                game_process_turn(&game, -1, 0);
                break;
            case INPUT_MOVE_NE:
                game_process_turn(&game, 1, -1);
                break;
            case INPUT_MOVE_NW:
                game_process_turn(&game, -1, -1);
                break;
            case INPUT_MOVE_SE:
                game_process_turn(&game, 1, 1);
                break;
            case INPUT_MOVE_SW:
                game_process_turn(&game, -1, 1);
                break;
            case INPUT_STAIRS:
                {
                    Tile* tile = map_get_tile(&game.map, game.player->x, game.player->y);
                    if (tile && tile->type == TILE_STAIRS_DOWN) {
                        game.floor_level++;
                        game.score += 50;
                        game_new_floor(&game);
                    } else {
                        strcpy(game.message, "There are no stairs here.");
                    }
                }
                break;
            case INPUT_NEW_GAME:
                if (game.state == STATE_GAME_OVER) {
                    game_init(&game);
                }
                break;
            case INPUT_QUIT:
                running = 0;
                break;
            default:
                break;
        }
    }
    
    render_shutdown();
    return 0;
}
