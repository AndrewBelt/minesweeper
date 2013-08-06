#include "game.h"
#include "config.h"
#include "hud.h"
#include <curses.h>


static void initialize_gui();
static void destroy_gui();


void game_create(struct game_t *game)
{
	game->dirty = 1;
}


void game_destroy(struct game_t *game)
{
}


void game_start(struct game_t *game)
{
	/* TEMP */
	grid_create(&game->grid, 10, 10);
	grid_seed(&game->grid, 0.10f);
	
	/* Set up the curses windows */
	
	initialize_gui();
	refresh();
	
	game->game_window = newwin(game->grid.height, game->grid.width * 2, 2, 0);
	game->hud_window = newwin(2, game->grid.width * 2, 0, 0);
	
	game->state = GAME_PLAYING;
	
	/* The game loop */
	
	while (game->state != GAME_QUIT)
	{
		game_loop(game);
	}
	
	/* Clean up */
	
	grid_destroy(&game->grid);
	delwin(game->hud_window);
	delwin(game->game_window);
	destroy_gui();
}


void game_loop(struct game_t *game)
{
	/* Redraw the grid if needed */
	
	if (game->dirty)
	{
		game_refresh(game);
		game->dirty = 0;
	}
	
	/* Block until a key or mouse button is pressed */
	int ch = getch();
	game_handle_key(game, ch);
}


void game_handle_key(struct game_t *game, int ch)
{
	switch (ch)
	{
		/* Controller keys (defined in config.h) */
		
		case CONTROL_DIG: game_dig(game);
			break;
		
		case CONTROL_FLAG:
			grid_toggle_flagged(&game->grid, game->cursor_x, game->cursor_y);
			game->dirty = 1;
			break;
		
		case CONTROL_QUESTION:
			grid_toggle_question(&game->grid, game->cursor_x, game->cursor_y);
			game->dirty = 1;
			break;
		
		case CONTROL_REVEAL: game_lose(game);
			break;
		
		case CONTROL_RESTART: game_restart(game);
			break;
		
		case CONTROL_QUIT: game_quit(game);
			break;
		
		/* Arrow keys */
		
		case KEY_LEFT: game_move_cursor(game, -1, 0);
			break;
		
		case KEY_RIGHT: game_move_cursor(game, 1, 0);
			break;
		
		case KEY_UP: game_move_cursor(game, 0, -1);
			break;
		
		case KEY_DOWN: game_move_cursor(game, 0, 1);
			break;
		
		case 'a': game_move_cursor(game, -wasd_speed, 0);
			break;
		
		case 'd': game_move_cursor(game, wasd_speed, 0);
			break;
		
		case 'w': game_move_cursor(game, 0, -wasd_speed);
			break;
		
		case 's': game_move_cursor(game, 0, wasd_speed);
			break;
		
		/* Non-keys */
		
		case KEY_MOUSE:
		{
			MEVENT event;
			getmouse(&event);
			game_handle_mouse(game, &event);
			break;
		}
	}
}


void game_handle_mouse(struct game_t *game, MEVENT *event)
{
	/* Get the grid coordinates of the click event */
	int win_x = event->x - getbegx(game->game_window);
	int win_y = event->y - getbegy(game->game_window);
	
	if (win_x % 2)
		return;
	
	/* Update the cursor position */
	game->cursor_x = win_x / 2;
	game->cursor_y = win_y;
	
	/* Handle the click by type */
	switch (event->bstate)
	{
		/* Left click */
		case BUTTON1_CLICKED:
			game_dig(game);
			break;
	}
}


void game_dig(struct game_t *game)
{
	if (grid_is_bomb(&game->grid, game->cursor_x, game->cursor_y))
	{
		game_lose(game);
	}
	else
	{
		grid_reveal_cell(&game->grid, game->cursor_x, game->cursor_y);
		
		if (grid_is_win(&game->grid))
			game_win(game);
	}
	
	game->dirty = 1;
}


void game_lose(struct game_t *game)
{
	grid_reveal_all(&game->grid);
	game->state = GAME_LOSE;
	game->dirty = 1;
}


void game_win(struct game_t *game)
{
	grid_reveal_all(&game->grid);
	game->state = GAME_WIN;
	game->dirty = 1;
}


void game_restart(struct game_t *game)
{
	/* TODO */
	/* Allow the difficulty to be changed */
	grid_seed(&game->grid, 0.10f);
	game->dirty = 1;
	game->state = GAME_PLAYING;
}


void game_refresh(struct game_t *game)
{
	hud_display(game);
	grid_display(&game->grid, game->game_window);
	game_update_cursor(game);
	refresh();
}


void game_quit(struct game_t *game)
{
	game->state = GAME_QUIT;
}


static inline int clamp_int(int value, int low, int high)
{
	value = (value < low) ? low : value;
	value = (value > high) ? high : value;
	return value;
}


void game_move_cursor(struct game_t *game, int dx, int dy)
{
	game->cursor_x += dx;
	game->cursor_y += dy;
	
	/* Clamp the cursor to bounds */
	
	game->cursor_x = clamp_int(game->cursor_x, 0, game->grid.width - 1);
	game->cursor_y = clamp_int(game->cursor_y, 0, game->grid.height - 1);
	
	game_update_cursor(game);
	refresh();
}


void game_update_cursor(struct game_t *game)
{
	int scr_x = getbegx(game->game_window) + game->cursor_x * 2;
	int scr_y = getbegy(game->game_window) + game->cursor_y;
	
	wmove(stdscr, scr_y, scr_x);
}


void initialize_gui()
{
	initscr();
	cbreak();
	keypad(stdscr, TRUE);
	noecho();
	mousemask(ALL_MOUSE_EVENTS, NULL);
	start_color();
	
	/* Color pairs */
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);
}


void destroy_gui()
{
	endwin();
}
