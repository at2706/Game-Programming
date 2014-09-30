#include "Entity.h"

SDL_Window* displayWindow;
const char GAME_NAME[] = "Classic Pong";
const GLint RESOLUTION_W = 800;
const GLint RESOLUTION_H = 600;
const Uint8 *keys = SDL_GetKeyboardState(NULL);

const GLfloat BALL_SPEED = 1.0f;
const GLfloat PADDLE_SPEED = 0.9f;

float lastFrameTicks = 0.0f;
float elapsed = 0.0f;

Entity ball("", 0.0f, 0.0f, 0.04f, 0.04f, 0.0f, 0.0f);
Entity paddle1("", -0.95f, 0.0f, 0.045f, 0.3f, 0.0f, PADDLE_SPEED);
Entity paddle2(paddle1);

typedef struct {
	float x;
	float y;

	float r;
	float g;
	float b;
} Vertex2D;

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

void DrawUIMain(){
	Vertex2D border[4] = { { -0.98f, 0.04f, 1.0f, 1.0f, 1.0f },
	{ -0.98f, -0.04f, 1.0f, 1.0f, 1.0f },
	{ 0.98f, -0.04f, 1.0f, 1.0f, 1.0f },
	{ 0.98f, 0.04f, 1.0f, 1.0f, 1.0f } };

	drawQuad(border, 0.0, 0.93f, 1.0f, 1.0f);
	drawQuad(border, 0.0, -0.93f, 1.0f, 1.0f);

	glDisableClientState(GL_COLOR_ARRAY);
}

GLfloat time(){
	static GLfloat lastFrameTicks = 0.0f;
	static GLfloat elapsed = 0;

	float ticks = (float)SDL_GetTicks() / 1000.0f;
	elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	return elapsed;
}

void Setup(){
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow(GAME_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, RESOLUTION_W, RESOLUTION_H, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	glViewport(0, 0, RESOLUTION_W, RESOLUTION_H);
	glMatrixMode(GL_MODELVIEW);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	ball.loadTexture("ballWhite.png");
	ball.facing = 25.0f;
	paddle2.setPos(0.95f, 0.0f);
}

bool ProcessEvents(){
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			return false;
		}
		
	}
	if (keys[SDL_SCANCODE_W] && paddle1.y < 0.68f) {
		paddle1.move(90.0f, elapsed);
	}
	else if (keys[SDL_SCANCODE_S] && paddle1.y > -0.68f) {
		paddle1.move(270.0f, elapsed);
	}
	if (keys[SDL_SCANCODE_UP] && paddle2.y < 0.68f) {
		paddle2.move(90.0f, elapsed);
	}
	else if (keys[SDL_SCANCODE_DOWN] && paddle2.y > -0.68f) {
		paddle2.move(270.0f, elapsed);
	}
	else if (keys[SDL_SCANCODE_SPACE] && ball.speed == 0.0f) {
		ball.speed = BALL_SPEED;
	}
	return true;
}

void Update(){
	//Paddle Collision
	if (ball.collision(paddle2) || ball.collision(paddle1)){
		ball.facing = 180.0f - ball.facing;
	}
	//Border Collision
	else if (ball.y < -0.85f || ball.y > 0.85f) ball.facing = -ball.facing;
	//Goal Collision
	else if (ball.x < -0.96f){
		ball.setPos(-0.9f, paddle1.y);
		ball.facing = rand() % 160 - 80;
		ball.speed = 0.0f;
	}
	else if (ball.x > 0.96f){
		ball.setPos(0.9f, paddle2.y);
		ball.facing = rand() % 160 + 110;
		ball.speed = 0.0f;
	}

	if (ball.speed == 0.0f){
		if (ball.x < 0) ball.y = paddle1.y;
		if (ball.x > 0) ball.y = paddle2.y;
	}

	ball.move(ball.facing, elapsed);

}

void Render(){
	ball.draw();
	paddle1.draw();
	paddle2.draw();
	DrawUIMain();

	glMatrixMode(GL_MODELVIEW);

	SDL_GL_SwapWindow(displayWindow);
}

void Cleanup(){
	SDL_Quit();
}


int main(int argc, char *argv[])
{
	Setup();
	
	while (ProcessEvents()) {
		
		glClear(GL_COLOR_BUFFER_BIT);

		elapsed = time();
		Update();

		Render();
	}

	Cleanup();
	
	return 0;
}