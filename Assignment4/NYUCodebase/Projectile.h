#pragma once
#include "Entity.h"
class Projectile :
	public Entity
{
public:
	Projectile(SheetSprite *sheet, vector<Entity*>::iterator id,
		GLfloat posX = 0.0f, GLfloat posY = 0.0f,
		GLfloat scale = 0.3f, GLfloat timedLife = 0.0f);
	~Projectile();

	GLvoid fixedUpdate(GLfloat gravity_x, GLfloat gravity_y);
	GLfloat lifeTime;

private:
	GLfloat timeBorn;
	vector<Entity*>::iterator owner;
};

