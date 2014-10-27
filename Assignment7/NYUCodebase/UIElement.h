#pragma once
#include "Utilities.h"
#include "SheetSprite.h"
class UIElement
{
public:
	UIElement();
	~UIElement();

	virtual GLvoid attach(UIElement *e);
	virtual GLvoid draw();

private:
	UIElement *parent;
	vector<UIElement> *children;
	SheetSprite *sprite;

	GLfloat x;
	GLfloat y;
	GLfloat width;
	GLfloat height;
};

