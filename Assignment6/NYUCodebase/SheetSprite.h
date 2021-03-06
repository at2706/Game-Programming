#pragma once
#include "Utilities.h"

class SheetSprite
{
public:
	SheetSprite();
	SheetSprite(GLuint textureID, GLfloat u, GLfloat v, GLfloat width, GLfloat height);
	~SheetSprite();

	virtual GLvoid draw(GLfloat x, GLfloat y, GLfloat facing, GLfloat scale);
	GLuint textureID;

	GLfloat u;
	GLfloat v;
	GLfloat width;
	GLfloat height;

private:
	friend class Entity;
	friend class GameEngine;

};

class SpriteUniformed : public SheetSprite{
public:
	SpriteUniformed(GLuint textureID, GLint index, GLint SpriteCountX, GLint SpriteCountY);
	GLvoid draw(GLfloat x, GLfloat y, GLfloat facing, GLfloat scale);

private:
	GLint index;
	GLint spriteCountX;
	GLint spriteCountY;
};
