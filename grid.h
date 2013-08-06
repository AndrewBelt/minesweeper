#ifndef GRID_H
#define GRID_H

#include <curses.h>


enum cell_state
{
	/* '#' */
	CELL_UNREVEALED,
	
	/* '*' or ' ' or an integer */
	CELL_REVEALED,
	
	/* '!' */
	CELL_FLAGGED,
	
	/* '?' */
	CELL_QUESTION
};


struct cell_t
{
	int bomb;
	enum cell_state state;
};


struct grid_t
{
	int width;
	int height;
	struct cell_t *cells;
	int total_bombs;
};


void grid_create(struct grid_t *grid, int width, int height);
void grid_destroy(struct grid_t *grid);

void grid_seed(struct grid_t *grid, float density);

struct cell_t *grid_get_cell(struct grid_t *grid, int x, int y);
int grid_is_bomb(struct grid_t *grid, int x, int y);
int grid_adjacent_bombs(struct grid_t *grid, int x, int y);

void grid_reveal_cell(struct grid_t *grid, int x, int y);

/* Toggles a cell between unrevealed, flagged, and question */
void grid_toggle_flagged(struct grid_t *grid, int x, int y);
void grid_toggle_question(struct grid_t *grid, int x, int y);

/* Changes the state of all cells to revealed */
void grid_reveal_all(struct grid_t *grid);

/* State-checking functions */

/* Checks if the game has been won */
int grid_is_win(struct grid_t *grid);

/* Counts the number of cells with a given state */
int grid_count_cells(struct grid_t *grid, enum cell_state state);

int grid_get_char(struct grid_t *grid, int x, int y);
void grid_print_cell(struct grid_t *grid, WINDOW *win, int x, int y);
void grid_display(struct grid_t *grid, WINDOW *win);

#endif