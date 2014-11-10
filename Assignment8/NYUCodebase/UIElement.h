#pragma once
#include "Utilities.h"
#include "SheetSprite.h"

class UIElement
{
public:
	UIElement(SheetSprite *sheet, GLfloat posX = 0.0f, GLfloat posY = 0.0f, GLfloat scale_x = 1.0f, GLfloat scale_y = 1.0f);
	~UIElement();

	virtual GLvoid attach(UIElement *e);
	virtual GLvoid draw();
	GLvoid buildMatrix();

protected:
	UIElement *parent;
	vector<UIElement*> children;
	SheetSprite *sprite;

	Matrix matrix;
	Vector position;
	Vector scale;
	GLboolean isVisible;
	
};