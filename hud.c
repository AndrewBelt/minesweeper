#include "hud.h"
#include "grid.h"


static const char *smilies[] =
{
	":-)",
	":-(",
	"B-)"
};


void hud_display(struct game_t *game)
{
	wmove(game->hud_window, 0, 0);
	wclrtobot(game->hud_window);
	
	/* Remaining flags */
	wmove(game->hud_window, 0, 0);
	int flags = grid_count_cells(&game->grid, CELL_FLAGGED);
	int bombs = game->grid.total_bombs;
	wprintw(game->hud_window, "%d", bombs - flags);
	
	/* Smiley */
	wmove(game->hud_window, 0, 5);
	const char *smiley = smilies[game->state];
	wprintw(game->hud_window, "%s", smiley);
	
	wrefresh(game->hud_window);
}
