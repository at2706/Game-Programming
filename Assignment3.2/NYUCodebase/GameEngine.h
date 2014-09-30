#pragma once
#include "Entity.h"
#define MAX_BULLETS 20
#define MAX_LASERS 8

using namespace std;
class GameEngine
{
public:
	GameEngine();
	~GameEngine();

	GLboolean ProcessEvents();
	GLvoid Update();
	GLvoid Render();

private:
	//Game Constants
	SDL_Window* displayWindow;
	const string GAME_NAME = "Space Invaders";
	const GLint RESOLUTION_W = 880;
	const GLint RESOLUTION_H = 660;
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	enum GameState { STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_GAME_OVER };
	GLvoid Setup();
	GLvoid time();
	GLuint loadTexture(const char *image_path, GLint param = GL_LINEAR);
	GLvoid DrawText(string text, GLfloat size, GLfloat spacing, GLfloat x, GLfloat y, GLfloat r, GLfloat g, GLfloat b, GLfloat a);
	GLvoid DrawBackground(GLfloat offsetX);
	GLfloat elapsed;
	GLfloat lastFrameTicks;
	GameState state;
	vector<Entity*> entities;
	SpriteSheet *charSheet;

	//Game Specific Stuff
	const GLint ALIEN_COL = 4;
	const GLint ALIEN_ROW = 12;
	const GLint ROW_MAX = 7;
	const GLfloat BULLET_SPEED = 0.9f;
	const GLfloat BULLET_DELAY = 0.4f;
	Entity *bullets[MAX_BULLETS];
	Entity *lasers[MAX_LASERS];

	GLuint fontTexture;
	GLuint bgTexture;

	GLint bulletIndex;
	GLfloat lastBullet;
	GLfloat alienSpeed;
	GLfloat alienDirection;
	GLfloat alienTravelTime;
	GLfloat alienFireDelay;
	GLfloat alienLastLaser;
	GLint alienLaserIndex;
	GLint alienFiringIndex;
	GLboolean movingRight;
	GLint alienRowCount;

	Entity *spaceship;
	Entity *startButton;

	GLint score;
};

