#pragma once
#include "Entity.h"
#include "Monster.h"
#include "Projectile.h"
#define MAX_ENEMIES 10
#define MAX_BULLETS 15

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
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	enum GameState { STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_GAME_OVER };
	GLvoid Setup();
	GLvoid time();
	GLuint loadTexture(const char *image_path, GLint param = GL_LINEAR);
	GLvoid DrawText(string text, GLfloat size, GLfloat spacing, GLfloat x, GLfloat y, GLfloat r, GLfloat g, GLfloat b, GLfloat a);
	GLvoid DrawBackground(GLfloat offsetX = 0.0f);
	GLvoid collisionPen();

	GLvoid drawPlatformHorizontal(GLfloat length, GLfloat x, GLfloat y);
	GLvoid drawPlatformVertical(GLfloat length, GLfloat x, GLfloat y);
	GLvoid reset();

	GLfloat timeLeftOver = 0.0f;
	GLfloat elapsed;
	GLfloat ticks;
	GLfloat lastFrameTicks;
	GLfloat gravity_x;
	GLfloat gravity_y;
	GameState state;

	Entity *bullets[MAX_BULLETS];
	Entity *enemies[MAX_ENEMIES];

	GLuint charSheet;
	GLuint fontTexture;
	GLuint bgTexture;
	GLint score;

	Entity *hero;

	//Game Specific Stuff
	const GLfloat BULLET_SPEED = 1.0f;
	const GLfloat BULLET_DELAY = 0.4f;
	const GLfloat ENEMY_DELAY = 0.6f;

	SheetSprite *placeHolderSprite;
	SheetSprite *bulletSprite;

	Entity *platform;
	Entity *button;
	Projectile *bullet;

	GLfloat lastBullet;
	GLint bulletIndex;

	GLfloat lastEnemy;
	GLint EnemyIndex;

};
