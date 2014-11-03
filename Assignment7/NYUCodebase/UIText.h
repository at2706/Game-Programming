#pragma once
#include "UIElement.h"
class UIText :
	public UIElement
{
public:
	UIText(GLfloat posX = 0.0f, GLfloat posY = 0.0f, GLfloat scale_x = 0.1f, GLfloat scale_y = 0.1f);
	~UIText();

	GLvoid setup(GLuint texID, string text, GLfloat spacing = 0.0f, GLfloat r = 0.0f, GLfloat g = 0.0f, GLfloat b = 0.0f, GLfloat a = 1.0f);
	GLvoid draw();

private:
	GLuint textureID;

	string text;
	GLfloat spacing;

	GLfloat r;
	GLfloat g;
	GLfloat b;
	GLfloat a;
};

