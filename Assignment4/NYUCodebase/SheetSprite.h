#pragma once
#include "Utilities.h"

class SheetSprite
{
public:
	SheetSprite(GLuint textureID, GLfloat u, GLfloat v, GLfloat width, GLfloat height );
	~SheetSprite();

	GLvoid draw(GLfloat x, GLfloat y, GLfloat scale = 1.0f);
	GLuint textureID;

private:
	friend class Entity;
	friend class GameEngine;
	GLfloat u;
	GLfloat v;
	GLfloat width;
	GLfloat height;
};