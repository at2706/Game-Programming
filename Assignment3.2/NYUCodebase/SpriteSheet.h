#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <math.h>
#include <stdlib.h>
#include <string>
#include <vector>
class SpriteSheet
{
public:
	SpriteSheet(const char *image_path);
	~SpriteSheet();

	GLvoid draw(GLfloat u, GLfloat v, GLfloat x, GLfloat y, GLfloat width, GLfloat height, GLfloat scale = 1.0f);

	GLuint textureID;
private:
	GLvoid loadTexture(const char *image_path);

};

