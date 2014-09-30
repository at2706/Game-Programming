#pragma once
#include "Entity.h"
using namespace std;
class GameEngine
{
public:
	GameEngine();
	~GameEngine();

	GLvoid time();
	GLvoid Setup();
	GLboolean ProcessEvents();
	GLvoid Update();
	GLvoid Render();

private:
	SDL_Window* displayWindow;
	const string GAME_NAME = "Space Invaders";
	const GLint RESOLUTION_W = 800;
	const GLint RESOLUTION_H = 600;
	const Uint8 *keys = SDL_GetKeyboardState(NULL);

	GLfloat elapsed;
	GLfloat lastFrameTicks;

};

