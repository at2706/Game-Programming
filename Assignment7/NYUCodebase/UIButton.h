#pragma once
#include "UIElement.h"
class UIButton :
	public UIElement
{
public:
	UIButton(GLfloat posX = 0.0f, GLfloat posY = 0.0f, GLfloat scale_x = 1.0f, GLfloat scale_y = 1.0f);
	~UIButton();

	GLvoid setup(GLvoid (*onClick)(), SheetSprite *down, SheetSprite *up);
	GLvoid *onClick;

private:
	SheetSprite *buttonDown;
	SheetSprite *buttonUp;
};

