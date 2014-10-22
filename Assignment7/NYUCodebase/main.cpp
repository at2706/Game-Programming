#include "GameEngine.h"

int main(int argc, char *argv[])
{
	GameEngine SpaceInvaders;
	
	while (SpaceInvaders.ProcessEvents()) {
		
		SpaceInvaders.Update();

		SpaceInvaders.Render();
	}

	return 0;
}