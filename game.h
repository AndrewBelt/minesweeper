#ifndef GAME_H
#define GAME_H

#include "grid.h"


enum game_state
{
	GAME_PLAYING,
	GAME_LOSE,
	GAME_WIN,
	GAME_QUIT
};


struct game_t
{
	struct grid_t grid;
	WINDOW *game_window;
	WINDOW *hud_window;
	enum game_state state;
	
	/* Whether the window needs an update */
	int dirty;
	
	int cursor_x;
	int cursor_y;
};


void game_create(struct game_t *game);
void game_destroy(struct game_t *game);

/* Event loop */

void game_start(struct game_t *game);
void game_loop(struct game_t *game);
void game_handle_key(struct game_t *game, int ch);

/* Game state */

void game_dig(struct game_t *game);
void game_lose(struct game_t *game);
void game_win(struct game_t *game);
void game_restart(struct game_t *game);
void game_quit(struct game_t *game);

void game_refresh(struct game_t *game);

void game_move_cursor(struct game_t *game, int dx, int dy);
void game_update_cursor(struct game_t *game);

#endif