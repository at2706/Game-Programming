#pragma once
#include "Utilities.h"
#include "SheetSprite.h"
class UIElement
{
public:
	UIElement();
	~UIElement();

	virtual GLvoid attach(UIElement e);

private:
	UIElement *parent;
	UIElement *children;
	SheetSprite *sprite;

	GLfloat x;
	GLfloat y;
	GLfloat width;
	GLfloat height;
};

