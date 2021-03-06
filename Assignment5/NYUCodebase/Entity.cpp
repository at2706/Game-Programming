#include "Entity.h"

vector<Entity*> Entity::entities;
Entity::Entity(SheetSprite *sheet, GLfloat posX, GLfloat posY,
				GLfloat speed, GLfloat mass, GLfloat scale, GLfloat health)
				: scale(scale), health(health){
	sprite = sheet;
	setPos(posX, posY);
	setMovement(speed);

	velocity_x = 0;
	isStatic = false;
	isIdle = true;
	isVisable = true;

	enableGravity = true;
	enableBounce = false;

	enableCollisions = true;
	collidedTop = false;
	collidedBottom = false;
	collidedLeft = false;
	collidedRight = false;

	entities.push_back(this);
	id = entities.end();
}

Entity::~Entity(){ entities.erase(id); }
GLvoid Entity::draw(){
	if (isVisable)
	sprite->draw(x, y, scale);

}
GLvoid Entity::drawAll(){
	for (vector<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
		(*it)->draw();
}
GLfloat Entity::lerp(float v0, float v1, float t) {
	return (1.0 - t)*v0 + t*v1;
}

GLvoid Entity::fixedUpdate(GLfloat gravity_x, GLfloat gravity_y){
	collidedTop = false;
	collidedBottom = false;
	collidedLeft = false;
	collidedRight = false;
	if (!isStatic){
		if (!isIdle){
			moveY();
			collisionPenY();
			moveX();
			collisionPenX();
		}
		else{
			decelerateY();
			collisionPenY();
			decelerateX();
			collisionPenX();
		}

		if (enableGravity && !collidedBottom){
			velocity_x += gravity_x * FIXED_TIMESTEP;
			velocity_y += gravity_y * FIXED_TIMESTEP;
		}
	}
}

GLvoid Entity::fixedUpdateAll(GLfloat gravity_x, GLfloat gravity_y){
	for (vector<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
		(*it)->fixedUpdate(gravity_x, gravity_y);
}

GLvoid Entity::setPos(GLfloat posX, GLfloat posY){
	x = posX;
	y = posY;
}

GLvoid Entity::setMovement(GLfloat s, GLfloat accelX, GLfloat accelY, GLfloat FricX, GLfloat FricY){
	speed = s;
	acceleration_x = accelX;
	acceleration_y = accelY;
	friction_x = FricX;
	friction_y = FricY;
}

GLvoid Entity::setRotation(GLfloat degree){
	rotation = degree;
}

GLvoid Entity::rotate(GLfloat degree){
	rotation += degree;
	if (rotation > 360.0f)	rotation -= 360.0f;
	else if (rotation < -360.0f) rotation += 360.0f;
}

GLvoid Entity::moveX(){
	GLfloat radian = (facing * PI) / 180.0f;

	velocity_x += acceleration_x * FIXED_TIMESTEP * cos(radian);

	if (velocity_x > speed) velocity_x = speed;
	else if (velocity_x < -speed) velocity_x = -speed;

	x += velocity_x * FIXED_TIMESTEP;

}

GLvoid Entity::moveY(){
	GLfloat radian = (facing * PI) / 180.0f;
	velocity_y += acceleration_y * FIXED_TIMESTEP * sin(radian);
	y += velocity_y * FIXED_TIMESTEP;
}

GLvoid Entity::decelerateX(){
	GLfloat radian = (facing * PI) / 180.0f;
	velocity_x = lerp(velocity_x, 0.0f, FIXED_TIMESTEP * friction_x);
	x += velocity_x * FIXED_TIMESTEP;
}

GLvoid Entity::decelerateY(){
	GLfloat radian = (facing * PI) / 180.0f;
	//velocity_y = lerp(velocity_y, 0.0f, FIXED_TIMESTEP * friction_y);
	y += velocity_y * FIXED_TIMESTEP;
}

//Box-box Collision
GLboolean Entity::collidesWith(Entity *e){
	if (!enableCollisions || !e->enableCollisions) return false;
	GLfloat top = y + ((sprite->height * scale) / 2);
	GLfloat bot = y - ((sprite->height * scale) / 2);
	GLfloat left = x - ((sprite->width * scale) / 2);
	GLfloat right = x + ((sprite->width * scale) / 2);

	GLfloat etop = e->y + ((e->sprite->height * e->scale) / 2);
	GLfloat ebot = e->y - ((e->sprite->height * e->scale) / 2);
	GLfloat eleft = e->x - ((e->sprite->width * e->scale) / 2);
	GLfloat eright = e->x + ((e->sprite->width * e->scale) / 2);

	return !(bot > etop || top < ebot || left > eright || right < eleft);
}
//Box-point Collision
GLboolean Entity::collidesWith(GLfloat posX, GLfloat posY){
	if (!enableCollisions) return false;
	GLfloat top = y + ((sprite->height * scale) / 2);
	GLfloat bot = y - ((sprite->height * scale) / 2);
	GLfloat left = x - ((sprite->width * scale) / 2);
	GLfloat right = x + ((sprite->width * scale) / 2);

	return !((posX > left && posX < right) || (posY > bot && posY < top));
}


GLvoid Entity::collisionPenY(){
	vector<Entity*>::iterator end = entities.end();
	for (vector<Entity*>::iterator it2 = entities.begin(); it2 != end; ++it2){
		if (this != (*it2) && (*it2)->isStatic && collidesWith((*it2))){
			GLfloat distance_y = fabs((*it2)->y - y);
			GLfloat height1 = sprite->height * 0.5f * scale;
			GLfloat height2 = (*it2)->sprite->height * 0.5f * (*it2)->scale;
			GLfloat yPenetration = fabs(distance_y - height1 - height2);

			if (y > (*it2)->y){
				y += yPenetration + 0.0001f;
				collidedBottom = true;
			}
			else{
				y -= yPenetration + 0.0001f;
				collidedTop = true;
			}

			//if (!(*it2)->isStatic) (*it2)->y += yPenetration;
			velocity_y = 0.0f;
		}
	}
}

GLvoid Entity::collisionPenX(){
	vector<Entity*>::iterator end = entities.end();
	for (vector<Entity*>::iterator it2 = entities.begin(); it2 != end; ++it2){
		if (this != (*it2) && (*it2)->isStatic && collidesWith((*it2))){
			GLfloat distance_x = fabs((*it2)->x - x);
			GLfloat width1 = sprite->width * 0.5f * scale;
			GLfloat width2 = (*it2)->sprite->width * 0.5f * (*it2)->scale;
			GLfloat xPenetration = fabs(distance_x - width1 - width2);

			if (x > (*it2)->x){
				x += xPenetration + 0.0001f;
				collidedRight = true;
			}
			else{
				x -= xPenetration + 0.0001f;
				collidedLeft = true;
			}

			velocity_x = 0.0f;
		}
	}
}