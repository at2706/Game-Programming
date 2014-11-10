#include "UIButton.h"


UIButton::UIButton(GLfloat posX, GLfloat posY, GLfloat scale_x, GLfloat scale_y)
	: UIElement(nullptr,posX,posY,scale_x,scale_y) {
}


UIButton::~UIButton()
{
}

GLvoid setup(GLvoid (*onClickFunc)(), SheetSprite *down, SheetSprite *up){
	//onClick = onClickFunc;
}