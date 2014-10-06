#include "Projectile.h"


Projectile::Projectile(SheetSprite *sheet, vector<Entity*>::iterator id, 
	GLfloat posX, GLfloat posY, GLfloat scale, GLfloat timedLife)
	: Entity(sheet, posX, posY, speed, 0, scale, 1), lifeTime(timedLife){
	enableGravity = false;
	enableCollisions = false;
	isIdle = false;
	owner = id;
	velocity_x = 1.0f;
	friction_x = 0.0f;
	friction_y = 0.0f;
	timeBorn = (GLfloat)SDL_GetTicks() / 1000.0f;

	Entity::entities.pop_back();
	Entity::entities.push_back(this);
	id = entities.end()-1;
}


Projectile::~Projectile()
{
}

GLvoid Projectile::fixedUpdate(GLfloat gravity_x, GLfloat gravity_y){
	if (lifeTime != 0.0f && (GLfloat)SDL_GetTicks() / 1000.0f - timeBorn > lifeTime)
		Entity::entities.erase(id);
	/*Entity::fixedUpdate(gravity_x, gravity_y);
	vector<Entity*>::iterator end = Entity::entities.end();

	for (vector<Entity*>::iterator it = Entity::entities.begin(); it != end; ++it){
		if (collidesWith((*it)->x, (*it)->y) && it != owner){
			return;
		}
	}*/
}
