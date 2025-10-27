#define SDL_MAIN_HANDLED
#include "game.h"

//just starting the game
int main() {
	Game game;
	bool success = game.initialize();
	if (success)
	{
		game.run();
	}

	game.shutdown();

	return 0;
}