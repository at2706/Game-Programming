#pragma once
#include "Utilities.h"
#include "SheetSprite.h"
#include "Matrix.h"
class Entity{
public:
	Entity(SheetSprite *sheet, GLfloat posX = 0.0f, GLfloat posY = 0.0f, 
		GLfloat speed = 0.5f, GLfloat mass = 1.0f, GLfloat scale = 1.0f, GLfloat health = 1.0f);
	~Entity();

	GLvoid draw();
	virtual GLvoid fixedUpdate(GameEngine *g);
	GLfloat lerp(GLfloat v0, GLfloat v1, GLfloat t);
	GLvoid buildMatrix();
	GLvoid worldToTileCoordinates(GLfloat worldX, GLfloat worldY, GLint *gridX, GLint *gridY);

	GLvoid setScale(GLfloat x, GLfloat y);
	GLvoid setPos(GLfloat posX, GLfloat posY);
	GLvoid setFacing(GLfloat degree, GLfloat f = 10.0f);
	GLvoid setMovement(GLfloat speed, GLfloat acceleration_x = 5.0f, GLfloat acceleration_y = 5.0f, 
					GLfloat friction_x = 5.0f, GLfloat friction_y = 5.0f);
	GLvoid setVelocity(GLfloat vx, GLfloat vy);
	GLvoid setAnimation(GLuint *arr);
	GLvoid rotate(GLfloat degree);

	GLvoid moveX();
	GLvoid moveY();

	GLvoid decelerateX();
	GLvoid decelerateY();

	GLvoid moveR();
	GLvoid decelerateR();

	GLboolean collidesWith(Entity *e);
	GLboolean collidesWith(GLfloat posX, GLfloat posY);
	GLboolean collidesWith(GLint tileX, GLint tileY);
	GLboolean collisionCheck(Entity *e);

	GLvoid collisionPenX();
	GLvoid collisionPenY();

	GLvoid tileCollisionY(GameEngine *g);
	GLvoid tileCollisionX(GameEngine *g);

	GLvoid collisionAxis(GameEngine *g);

	static vector<Entity*> entities;
	static GLvoid drawAll();
	static GLvoid fixedUpdateAll(GameEngine *g);

	Matrix matrix;

	GLfloat x;
	GLfloat y;
	GLint gridX;
	GLint gridY;

	string msg1;
	string msg2;

protected:
	friend class GameEngine;
	vector<Entity*>::iterator id;
	SheetSprite *sprite;
	GLuint *animationFrames;
	GLuint animationIndex;
	GLfloat animationElapsed;
	GLfloat framesPerSecond;
	GLfloat rotation;

	GLfloat scale_x;
	GLfloat scale_y;

	GLfloat facing;
	GLfloat speed;
	GLfloat health;

	GLfloat velocity_x;
	GLfloat velocity_y;
	GLfloat acceleration_x;
	GLfloat acceleration_y;
	GLfloat friction_x;
	GLfloat friction_y;

	GLfloat velocity_r;
	GLfloat acceleration_r;
	GLfloat friction_r;

	GLfloat mass;
	GLboolean isStatic;
	GLboolean isIdle;
	GLboolean isVisible;
	GLboolean rotating;

	GLboolean enableGravity;
	GLboolean enableBounce;

	GLboolean enableCollisions;
	GLboolean collidedTop;
	GLboolean collidedBottom;
	GLboolean collidedLeft;
	GLboolean collidedRight;
};