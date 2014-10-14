#include "Monster.h"


Monster::Monster(SheetSprite *sheet, GLfloat posX, GLfloat posY,
	GLfloat speed, GLfloat mass, GLfloat scale, GLfloat health)
	: Entity(sheet, posX, posY, speed, mass, scale, health){
	isIdle = false;

	Entity::entities.pop_back();
	Entity::entities.push_back(this);
}


Monster::~Monster()
{
}

GLvoid Monster::fixedUpdate(GLfloat gravity_x, GLfloat gravity_y){
	Entity::fixedUpdate(gravity_x, gravity_y);
	if (collidedLeft || collidedRight)
		facing = facing == 180.0f ? 0.0f : 180.0f;

	if (y < -1.3f){
		y = 0.8f * (rand() % 3 + 2);
		x = -0.4f + ( 0.8f * (GLint)(rand() % 2));
	}
}
