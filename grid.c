#include "grid.h"
#include <stdlib.h>
#include <time.h>
#include <assert.h>


void grid_create(struct grid_t *grid, int width, int height)
{
	grid->width = width;
	grid->height = height;
	
	int cells_len = width * height;
	grid->cells = calloc(sizeof(struct cell_t), cells_len);
}


void grid_destroy(struct grid_t *grid)
{
	free(grid->cells);
}


void grid_seed(struct grid_t *grid, float density)
{
	int i;
	int cells_len = grid->width * grid->height;
	grid->total_bombs = 0;
	
	srand(time(NULL) + clock());
	
	/* Fill the grid with random bombs */
	
	for (i = 0; i < cells_len; i++)
	{
		struct cell_t *cell = &grid->cells[i];
		
		int bomb = ((float) rand() / RAND_MAX < density);
		cell->bomb = bomb;
		cell->state = CELL_UNREVEALED;
		
		grid->total_bombs += bomb;
	}
}


struct cell_t *grid_get_cell(struct grid_t *grid, int x, int y)
{
	/* Return NULL if out of bounds */
	
	if (x < 0 || grid->width <= x)
		return NULL;
	
	if (y < 0 || grid->height <= y)
		return NULL;
	
	int index = x + y * grid->width;
	return &grid->cells[index];
}


int grid_is_bomb(struct grid_t *grid, int x, int y)
{
	struct cell_t *cell = grid_get_cell(grid, x, y);
	return cell && (cell->bomb == 1);
}


int grid_adjacent_bombs(struct grid_t *grid, int x, int y)
{
	int dx;
	int dy;
	int bombs = 0;
	
	for (dy = -1; dy <= 1; dy++)
	{
		for (dx = -1; dx <= 1; dx++)
		{
			/* Don't count the cell itself */
			if (dx == 0 && dy == 0)
				continue;
			
			bombs += grid_is_bomb(grid, x + dx, y + dy);
		}
	}
	
	return bombs;
}


void grid_reveal_cell(struct grid_t *grid, int x, int y)
{
	struct cell_t *cell = grid_get_cell(grid, x, y);
	
	if (!cell)
		return;
	
	/* The cell must be unrevealed as a user protection */
	if (cell->state != CELL_UNREVEALED)
		return;
	
	cell->state = CELL_REVEALED;
	
	int bombs = grid_adjacent_bombs(grid, x, y);
	if (bombs > 0)
		return;
	
	/* Uncover adjacent cells recursively */
	
	int dx;
	int dy;
	
	for (dy = -1; dy <= 1; dy++)
	{
		for (dx = -1; dx <= 1; dx++)
		{
			grid_reveal_cell(grid, x + dx, y + dy);
		}
	}
}


void grid_toggle_flagged(struct grid_t *grid, int x, int y)
{
	struct cell_t *cell = grid_get_cell(grid, x, y);
	
	if (!cell)
		return;
	
	switch (cell->state)
	{
		case CELL_UNREVEALED:
		case CELL_QUESTION:
			cell->state = CELL_FLAGGED;
			break;
		
		case CELL_FLAGGED:
			cell->state = CELL_UNREVEALED;
			break;
		
		default:
			break;
	}
}


void grid_toggle_question(struct grid_t *grid, int x, int y)
{
	struct cell_t *cell = grid_get_cell(grid, x, y);
	
	if (!cell)
		return;
	
	switch (cell->state)
	{
		case CELL_UNREVEALED:
		case CELL_FLAGGED:
			cell->state = CELL_QUESTION;
			break;
		
		case CELL_QUESTION:
			cell->state = CELL_UNREVEALED;
			break;
		
		default:
			break;
	}
}


void grid_reveal_all(struct grid_t *grid)
{
	int cells_len = grid->width * grid->height;
	int index;
	
	for (index = 0; index < cells_len; index++)
	{
		struct cell_t *cell = &grid->cells[index];
		cell->state = CELL_REVEALED;
	}
}


int grid_is_win(struct grid_t *grid)
{
	int cells_len = grid->width * grid->height;
	int index;
	
	/* Check that all non-bomb cells are revealed */
	
	for (index = 0; index < cells_len; index++)
	{
		struct cell_t *cell = &grid->cells[index];
		
		if (!(cell->state == CELL_REVEALED || cell->bomb))
			return 0;
	}
	
	return 1;
}


int grid_count_cells(struct grid_t *grid, enum cell_state state)
{
	int cells_len = grid->width * grid->height;
	int index;
	int count = 0;
	
	for (index = 0; index < cells_len; index++)
	{
		struct cell_t *cell = &grid->cells[index];
		
		if (cell->state == state)
			count++;
	}
	
	return count;
}


int grid_get_char(struct grid_t *grid, int x, int y)
{
	struct cell_t *cell = grid_get_cell(grid, x, y);
	assert(cell);
	
	switch (cell->state)
	{
		case CELL_UNREVEALED:
			return ACS_BULLET;
		
		case CELL_REVEALED:
		{
			if (cell->bomb)
			{
				return '*' | A_BOLD | COLOR_PAIR(1);
			}
			
			int bombs = grid_adjacent_bombs(grid, x, y);
			
			if (bombs > 0)
			{
				char c[2];
				snprintf(c, 2, "%d", bombs);
				return c[0];
			}
			else
			{
				return ' ';
			}
		}
		
		case CELL_FLAGGED:
			return 'o' | COLOR_PAIR(2);
		
		case CELL_QUESTION:
			return '?' | COLOR_PAIR(3);
		
		default:
			return 0;
	}
}


void grid_print_cell(struct grid_t *grid, WINDOW *window, int x, int y)
{
	int scr_x = 2 * x + 2;
	int scr_y = y + 1;
	int cell_char = grid_get_char(grid, x, y);
	mvwaddch(window, scr_y, scr_x, cell_char);
}


void grid_display(struct grid_t *grid, WINDOW *window)
{
	box(window, 0, 0);
	
	int x;
	int y;
	
	for (y = 0; y < grid->height; y++)
	{
		for (x = 0; x < grid->width; x++)
		{
			grid_print_cell(grid, window, x, y);
		}
	}
	
	wrefresh(window);
}


WINDOW *grid_new_window(struct grid_t *grid)
{
	int x = 0;
	int y = 2;
	int width = grid->width * 2 + 3;
	int height = grid->height + 2;
	
	return newwin(height, width, y, x);
}
