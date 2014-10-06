#include "SpriteSheet.h"


SpriteSheet::SpriteSheet(const char *image_path) {
	loadTexture(image_path);
}


SpriteSheet::~SpriteSheet()
{
}

void SpriteSheet::loadTexture(const char *image_path){
	SDL_Surface *surface = IMG_Load(image_path);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	if (surface != NULL)
		glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	SDL_FreeSurface(surface);
}

GLvoid SpriteSheet::draw(GLfloat u, GLfloat v, GLfloat x, GLfloat y, GLfloat width, GLfloat height, GLfloat scale){

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x, y, 0.0);
	GLfloat quad[] = { -width * scale / 2, height * scale * 1.33f / 2, -width * scale / 2, -height * scale * 1.33f / 2,
		width * scale / 2, -height * scale * 1.33f / 2, width * scale / 2, height * scale * 1.33f / 2 };
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);
	GLfloat quadUVs[] = { u, v, u, v + height, u + width, v + height, u + width, v };
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
}