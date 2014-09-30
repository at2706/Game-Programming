#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <math.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <algorithm>
#include "SpriteSheet.h"
#define PI 3.14159265359f
using namespace std;
class Entity
{
public:
	Entity(SpriteSheet *sheet, GLfloat u, GLfloat v, GLfloat posX = 0.0f,
		GLfloat posY = 0.0f, GLfloat width = 1.0f, GLfloat height = 1.0f,
		GLfloat rotate = 0.0f, GLfloat speed = 1.0f, GLfloat scale = 1.0f);
	Entity(const Entity& e);
	~Entity();

	void draw();
	void loadTexture(const char *textureID);
	void setSize(GLfloat w, GLfloat h);
	void setPos(GLfloat posX, GLfloat posY);
	void setRotation(GLfloat degree);
	
	void rotate(GLfloat degree);

	void move(GLfloat degree, GLfloat e);
	GLboolean collision(Entity *e);
	GLboolean collision(GLfloat posX, GLfloat posY);
	
	GLfloat x;
	GLfloat y;
	GLfloat facing;
	GLfloat speed;
	GLboolean dead;
private:	

	SpriteSheet *sprite;

	GLfloat rotation;

	GLfloat width;
	GLfloat height;
	GLfloat u;
	GLfloat v;
	GLfloat scale;
};

