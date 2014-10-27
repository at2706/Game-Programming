#include "UIElement.h"


UIElement::UIElement(){
	parent = nullptr;
}


UIElement::~UIElement()
{
}

GLvoid UIElement::attach(UIElement *e){
	e->parent = this;
	children->push_back(*e);
}

GLvoid UIElement::draw(){
	GLfloat posX = 0, posY = 0;
	if (parent != nullptr){

	}
}