#pragma once
#include "Utilities.h"
#include "SheetSprite.h"
using namespace std;
class Entity
{
public:
	Entity(SheetSprite *sheet, GLfloat posX = 0.0f, GLfloat posY = 0.0f, 
		GLfloat speed = 0.5f, GLfloat mass = 1.0f, GLfloat scale = 1.0f, GLfloat health = 1.0f);
	~Entity();

	GLvoid draw();
	virtual GLvoid fixedUpdate(GLfloat gravity_x, GLfloat gravity_y);
	GLfloat lerp(float v0, float v1, float t);

	GLvoid setSize(GLfloat w, GLfloat h);
	GLvoid setPos(GLfloat posX, GLfloat posY);
	GLvoid setRotation(GLfloat degree);
	GLvoid setMovement(GLfloat speed, GLfloat acceleration_x = 0.5f, GLfloat acceleration_y = 0.5f,
					GLfloat friction_x = 0.8f, GLfloat friction_y = 0.8f);
	GLvoid rotate(GLfloat degree);

	GLvoid moveX();
	GLvoid moveY();

	GLvoid decelerateX();
	GLvoid decelerateY();

	GLboolean collidesWith(Entity *e);
	GLboolean collidesWith(GLfloat posX, GLfloat posY);

	GLvoid collisionPenX();
	GLvoid collisionPenY();


	static vector<Entity*> entities;
	static GLvoid drawAll();
	static GLvoid fixedUpdateAll(GLfloat gravity_x, GLfloat gravity_y);

	GLfloat x;
	GLfloat y;

protected:	
	friend class GameEngine;
	vector<Entity*>::iterator id;
	SheetSprite *sprite;
	GLfloat rotation;
	GLfloat scale;

	GLfloat facing;
	GLfloat speed;
	GLfloat health;

	GLfloat velocity_x;
	GLfloat velocity_y;
	GLfloat acceleration_x;
	GLfloat acceleration_y;
	GLfloat friction_x;
	GLfloat friction_y;
	GLfloat mass;
	GLboolean isStatic;
	GLboolean isIdle;
	GLboolean isVisable;

	GLboolean enableGravity;
	GLboolean enableBounce;

	GLboolean enableCollisions;
	GLboolean collidedTop;
	GLboolean collidedBottom;
	GLboolean collidedLeft;
	GLboolean collidedRight;
};

