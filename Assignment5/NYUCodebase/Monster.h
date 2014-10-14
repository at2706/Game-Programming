#pragma once
#include "Entity.h"
class Monster : public Entity {
public:
	Monster(SheetSprite *sheet, GLfloat posX = 0.0f, GLfloat posY = 0.0f,
		GLfloat speed = 0.5f, GLfloat mass = 1.0f, GLfloat scale = 1.0f, GLfloat health = 1.0f);
	~Monster();

	GLvoid fixedUpdate(GLfloat gravity_x, GLfloat gravity_y);
};

