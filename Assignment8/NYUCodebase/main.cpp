#include "GameEngine.h"

int main(int argc, char *argv[])
{
	GameEngine game;
	
	while (game.ProcessEvents()) {
		
		game.Update();

		game.Render();
	}

	return 0;
}