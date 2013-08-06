#include "help.h"
#include "config.h"
#include <curses.h>


void print_help()
{
	def_prog_mode();
	endwin();
	
	printf("arrow keys move\n");
	printf("wasd moves by %d cells\n", WASD_SPEED);
	printf("'%c'\tdig cell\n", CONTROL_DIG);
	printf("'%c'\ttoggle flag\n", CONTROL_FLAG);
	printf("'%c'\ttoggle question\n", CONTROL_QUESTION);
	printf("'%c'\treveal all cells\n", CONTROL_REVEAL);
	printf("'%c'\trestart\n", CONTROL_RESTART);
	printf("'%c'\tquit\n", CONTROL_QUIT);
	printf("press enter to continue\n");
	
	getch();
	reset_prog_mode();
	refresh();
}
