#include "SheetSprite.h"


SheetSprite::SheetSprite(GLuint textureID, GLfloat u, GLfloat v, GLfloat width, GLfloat height)
	: textureID(textureID), u(u), v(v), width(width), height(height){
	
}


SheetSprite::~SheetSprite()
{
}

GLvoid SheetSprite::draw(GLfloat x, GLfloat y, GLfloat scale){

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x, y, 0.0);
	GLfloat quad[] = {	-width * scale / 2, height * scale / 2,
						-width * scale / 2, -height * scale / 2,
						width * scale / 2, -height * scale / 2,
						width * scale / 2, height * scale / 2 };
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);
	GLfloat quadUVs[] = { u, v, u, v + height, u + width,
		v + height, u + width, v };
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
}