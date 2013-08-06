#include "game.h"


int main(int argc, const char *argv[])
{
	struct game_t game;
	
	game_create(&game);
	game_start(&game);
	game_destroy(&game);
	
	return 0;
}
