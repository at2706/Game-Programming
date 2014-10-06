#include "Entity.h"

Entity::Entity(SpriteSheet *sheet, GLfloat u, GLfloat v, GLfloat posX, GLfloat posY, GLfloat width, GLfloat height, GLfloat rotate, GLfloat speed, GLfloat scale)
	: speed(speed), scale(scale), rotation(rotate),u(u),v(v){
	sprite = sheet;
	setPos(posX, posY);
	setSize(width, height);
	health = 1;
}

Entity::Entity(const Entity& e) : x(e.x), y(e.y), width(e.width), 
height(e.height), rotation(e.rotation), speed(e.speed){}

Entity::~Entity(){

}

void Entity::draw(){
	sprite->draw(u, v, x, y, width, height, scale);

}

void Entity::setSize(GLfloat w, GLfloat h){
	width = w;
	height = h;
}

void Entity::setPos(GLfloat posX, GLfloat posY){
	x = posX;
	y = posY;
}

void Entity::setRotation(GLfloat degree){
	rotation = degree;
}

void Entity::rotate(GLfloat degree){
	rotation += degree;
	if (rotation > 360.0f)	rotation -= 360.0f;
	else if (rotation < -360.0f) rotation += 360.0f;
}

void Entity::move(GLfloat degree, GLfloat e){
	GLfloat radian = (degree * PI) / 180.0f;

	x += cos(radian) * e * speed;
	y += sin(radian) * e * speed;
}
//Box-box collision
GLboolean Entity::collision(Entity *e){
	GLfloat top = y + ((height * scale) / 2);
	GLfloat bot = y - ((height * scale) / 2);
	GLfloat left = x - ((width * scale) / 2);
	GLfloat right = x + ((width * scale) / 2);

	GLfloat etop = e->y + ((e->height * e->scale) / 2);
	GLfloat ebot = e->y - ((e->height * e->scale) / 2);
	GLfloat eleft = e->x - ((e->width * e->scale) / 2);
	GLfloat eright = e->x + ((e->width * e->scale) / 2);

	return !(bot > etop || top < ebot || left > eright || right < eleft);
}
//Box-point Collision
GLboolean Entity::collision(GLfloat posX, GLfloat posY){
	GLfloat top = y + ((height * scale) / 2);
	GLfloat bot = y - ((height * scale) / 2);
	GLfloat left = x - ((width * scale) / 2);
	GLfloat right = x + ((width * scale) / 2);

	return !((posX > left && posX < right) || (posY > bot && posY < top));
}