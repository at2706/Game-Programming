#include "UIElement.h"

UIElement::UIElement(SheetSprite *sheet, GLfloat posX, GLfloat posY, GLfloat scale_x, GLfloat scale_y)
	: isVisible(true), x(posX), y(posY), scale_x(scale_x), scale_y(scale_y){
	parent = nullptr;
	sprite = sheet;
}

UIElement::~UIElement(){
}

GLvoid UIElement::attach(UIElement *e){
	e->parent = this;
	children.push_back(e);
}

GLvoid UIElement::draw(){
	if (isVisible){
		
		if (parent != nullptr){
			GLfloat posX = 0, posY = 0;
			posX = (x * parent->sprite->width * parent->scale_x) + parent->x;
			posY = (y * parent->sprite->height /2 * parent->scale_y) + parent->y;

			sprite->draw(posX, posY, 0, scale_x, scale_y);

		}
		else{
			sprite->draw(x, y, 0, scale_x, scale_y);
		}

		for (vector<UIElement*>::iterator it = children.begin(); it != children.end(); ++it)
			(*it)->draw();
	}
}