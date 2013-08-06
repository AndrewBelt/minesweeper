#include "help.h"
#include "config.h"


void print_help(WINDOW *win)
{
	wmove(win, 0, 0);
	wprintw(win, "controls:\n");
	wprintw(win, "move with the arrow keys\n");
	wprintw(win, "%c\tdig cell\n", CONTROL_DIG);
	wprintw(win, "%c\ttoggle flag\n", CONTROL_FLAG);
	wprintw(win, "%c\ttoggle question\n", CONTROL_QUESTION);
	wprintw(win, "%c\treveal all cells\n", CONTROL_REVEAL);
	wprintw(win, "%c\trestart\n", CONTROL_RESTART);
	wprintw(win, "%c\tquit\n", CONTROL_QUIT);
}
