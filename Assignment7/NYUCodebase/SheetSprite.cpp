#include "SheetSprite.h"

SheetSprite::SheetSprite()
{
}

SheetSprite::~SheetSprite()
{
}

SheetSprite::SheetSprite(GLuint texID, GLfloat u, GLfloat v, GLfloat width, GLfloat height)
	: textureID(texID), u(u), v(v), width(width), height(height){
}

GLvoid SheetSprite::draw(GLfloat x, GLfloat y, GLfloat facing, GLfloat scaleX, GLfloat scaleY){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);
	GLfloat quad[] = {-width / 2, height / 2,
		-width / 2, -height / 2,
		width / 2, -height / 2,
		width / 2, height / 2 };
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

SpriteUniformed::SpriteUniformed(GLuint texID, GLint index, GLint SpriteCountX, GLint SpriteCountY)
	: index(index), spriteCountX(SpriteCountX), spriteCountY(SpriteCountY){
	textureID = texID;
	u = (GLfloat)(((GLint)index) % spriteCountX) / (GLfloat)spriteCountX;
	v = (GLfloat)(((GLint)index) / spriteCountX) / (GLfloat)spriteCountY;


	width = (spriteCountX / spriteCountY) / (GLfloat)spriteCountX;
	height = 1.0f / (GLfloat)spriteCountY;
}

GLvoid SpriteUniformed::draw(GLfloat x, GLfloat y, GLfloat facing, GLfloat scaleX, GLfloat scaleY){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);

	GLfloat quad[] = {-width * scaleX / 2, height * scaleY / 2,
		 -width * scaleX / 2, -height * scaleY / 2,
		width * scaleX / 2, -height * scaleY / 2,
		width * scaleX / 2, height * scaleY / 2 };
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat quadUVs[] = {	u, v,
							u, v + height,
							u + (width / (spriteCountX / spriteCountY)), v + height,
							u + (width / (spriteCountX / spriteCountY)), v };

	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
}