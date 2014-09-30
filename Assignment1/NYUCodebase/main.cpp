#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <iostream>

SDL_Window* displayWindow;

const GLint RESOLUTION_W = 800;
const GLint RESOLUTION_H = 600;

typedef struct {
	float x;
	float y;

	float r;
	float g;
	float b;
} Vertex2D;
//DRAW FUNCTIONS: parameters: x and y offsets, x and y scale
void drawQuad(Vertex2D* q, GLfloat x, GLfloat y, GLfloat sx, GLfloat sy){

	glLoadIdentity();
	glTranslatef(x, y, 0.0);
	glScalef(sx, sy, 1.0f);

	glVertexPointer(2, GL_FLOAT, sizeof(float) * 5, q);
	glEnableClientState(GL_VERTEX_ARRAY);
	glColorPointer(3, GL_FLOAT, sizeof(float) * 5, &q[0].r);
	glEnableClientState(GL_COLOR_ARRAY);
	glDrawArrays(GL_QUADS, 0, 4);
}

void drawTriangle(Vertex2D* t, GLfloat x, GLfloat y, GLfloat sx, GLfloat sy){
	glLoadIdentity();
	glTranslatef(x, y, 0.0);
	glScalef(sx, sy, 1.0);

	glVertexPointer(2, GL_FLOAT, sizeof(float) * 5, t);
	glEnableClientState(GL_VERTEX_ARRAY);
	glColorPointer(3, GL_FLOAT, sizeof(float) * 5, &t[0].r);
	glEnableClientState(GL_COLOR_ARRAY);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void drawHouse(GLfloat x, GLfloat y){
	Vertex2D triangle[3] = { { 0.0, 0.5, 0.65f, 0.16f, 0.16f },
	{ -0.5, -0.5, 0.65f, 0.16f, 0.16f },
	{ 0.5, -0.5, 0.65f, 0.16f, 0.16f } };
	drawTriangle(triangle, x, y + 0.5f, 1.1f, 0.5f);

	Vertex2D housebase[4] = { { -0.5f, 0.5f, 0.26f, 0.29f, 0.17f },
	{ -0.5f, -0.5f, 0.26f, 0.29f, 0.17f },
	{ 0.5f, -0.5f, 0.56f, 0.59f, 0.47f },
	{ 0.5f, 0.5f, 0.56f, 0.59f, 0.47f } };
	drawQuad(housebase, x, y, 1.0f, 0.5f);

	Vertex2D windowborder[4] = { { -0.5f, 0.5f, 0.0f, 0.0f, 0.0f },
	{ -0.5f, -0.5f, 0.0f, 0.0f, 0.0f },
	{ 0.5f, -0.5f, 0.0f, 0.0f, 0.0f },
	{ 0.5f, 0.5f, 0.0f, 0.0f, 0.0f } };
	drawQuad(windowborder, x + 0.2f, y, 0.2f, 0.25f);

	Vertex2D windowglass1[4] = { { -0.5f, 0.5f, 0.72f, 0.53f, 0.04f },
	{ -0.5f, -0.5f, 0.72f, 0.53f, 0.04f },
	{ 0.5f, -0.5f, 0.72f, 0.53f, 0.04f },
	{ 0.5f, 0.5f, 0.72f, 0.53f, 0.04f } };
	drawQuad(windowglass1, x + 0.245f, y, 0.075f, 0.2f);

	Vertex2D windowglass2[4] = { { -0.5f, 0.5f, 0.72f, 0.53f, 0.04f },
	{ -0.5f, -0.5f, 0.72f, 0.53f, 0.04f },
	{ 0.5f, -0.5f, 0.72f, 0.53f, 0.04f },
	{ 0.5f, 0.5f, 0.72f, 0.53f, 0.04f } };
	drawQuad(windowglass2, x + 0.155f, y, 0.075f, 0.2f);

	glDisableClientState(GL_COLOR_ARRAY);
}

GLuint LoadTexture(const char *image_path) {
	SDL_Surface *surface = IMG_Load(image_path);
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	if(surface != NULL) glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	SDL_FreeSurface(surface);
	return textureID;
}

void DrawSprite(GLint texture, float x, float y, float sx, float sy, float rotation) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x, y, 0.0);
	glScalef(sx, sy, 0.0f);
	glRotatef(rotation, 0.0, 0.0, 1.0);
	GLfloat quad[] = { -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f };
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);
	GLfloat quadUVs[] = { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
}

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO/* | SDL_INIT_JOYSTICK*/);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, RESOLUTION_W, RESOLUTION_H, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	bool done = false;
	
	glViewport(0, 0, RESOLUTION_W, RESOLUTION_H);
	glMatrixMode(GL_MODELVIEW);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);

	glClearColor(0.4f, 0.2f, 0.4f, 1.0f);

	float lastFrameTicks = 0.0f;
	float posMoonX = -0.8f;
	float posMoonY = 0.3f;

	GLuint grass = LoadTexture("grass.png");
	GLuint moon = LoadTexture("moon.png");
	GLuint sun = LoadTexture("sun.png");

	GLuint flyingObject = moon;

	SDL_Event event;

	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
			/*else if (event.type == SDL_MOUSEBUTTONDOWN){
				if (event.button.button = 1){}
			}*/
		}
		glClear(GL_COLOR_BUFFER_BIT);

		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;

		posMoonX += elapsed * 0.1f;
		if (posMoonX <= 0.0f) posMoonY += elapsed * 0.05f;
		else posMoonY -= elapsed * 0.05f;
		
		if (posMoonX > 1.25f){
			if (flyingObject == moon){
				flyingObject = sun;
				glClearColor(1.0f, 0.8f, 0.0f, 1.0f);
			}
			else{
				flyingObject = moon;
				glClearColor(0.4f, 0.2f, 0.4f, 1.0f);
			}
			posMoonX = -1.25f;
			posMoonY = 0.3f;
		}

		DrawSprite(flyingObject, posMoonX, posMoonY, 0.5f, 0.6f, 0.0f);
		

		/*GLfloat triangle[] = { 0.0f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f };
		glVertexPointer(2, GL_FLOAT, 0, triangle);
		glEnableClientState(GL_VERTEX_ARRAY);
		GLfloat triangleColors[] = { 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0 };
		glColorPointer(3, GL_FLOAT, 0, triangleColors);
		glEnableClientState(GL_COLOR_ARRAY);
		glDrawArrays(GL_TRIANGLES, 0, 3);*/
		
		drawHouse(-0.3f, -0.75f);
		DrawSprite(grass, -0.0f, -0.9f, 2.0f, 0.2f, 0.0f);

		/*SDL_Surface *surface = IMG_Load("grass.png");
		GLuint textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);*/

		
		

		glMatrixMode(GL_MODELVIEW);

		SDL_GL_SwapWindow(displayWindow);
	}

	SDL_Quit();
	return 0;
}