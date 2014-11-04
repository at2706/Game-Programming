#pragma once
#include "Entity.h"
#include "UIElement.h"
#include "UIText.h"

#define MAX_ENEMIES 1
#define MAX_BULLETS 15

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
	friend class Entity;
	SDL_Window* displayWindow;
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	enum GameState { STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_GAME_OVER };
	unsigned char **levelData;

	Mix_Chunk *jumpSound;

	GLvoid Setup();
	GLvoid time();
	GLuint loadTexture(const char *image_path, GLint param = GL_LINEAR);
	GLvoid DrawText(string text, GLfloat size, GLfloat spacing, GLfloat x, GLfloat y, GLfloat r, GLfloat g, GLfloat b, GLfloat a);
	GLvoid DrawBackground(GLfloat offsetX = 0.0f);

	GLvoid buildLevel();
	GLvoid drawLevel();
	
	GLboolean readHeader(ifstream &stream);
	GLboolean readLayerData(ifstream &stream);
	GLboolean readEntityData(ifstream &stream);
	GLvoid placeEntity(string &type, GLfloat x, GLfloat y);
	GLboolean isSolidTile(unsigned char t);
		
	GLvoid drawPlatformHorizontal(GLfloat length, GLfloat x, GLfloat y);

	GLvoid reset();
	
	GLint mapWidth;
	GLint mapHeight;

	GLfloat timeLeftOver = 0.0f;
	GLfloat elapsed;
	GLfloat ticks;
	GLfloat lastFrameTicks;
	GLfloat gravity_x;
	GLfloat gravity_y;
	GameState state;

	Entity *bullets[MAX_BULLETS];
	Entity *enemies[MAX_ENEMIES];

	GLuint tileSheet;
	GLuint UISheet;
	GLuint fontTexture;
	GLuint bgTexture;
	GLint score;

	Entity *hero;

	//Game Specific Stuff
	UIElement *UImain;
};