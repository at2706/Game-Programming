#include "GameEngine.h"


GameEngine::GameEngine(){
	Setup();
}


GameEngine::~GameEngine(){
	SDL_Quit();
}


GLvoid GameEngine::time(){
	GLfloat ticks = (GLfloat)SDL_GetTicks() / 1000.0f;
	elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;
}
GLvoid GameEngine::Setup(){
	//DO NOT TOUCH
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow(GAME_NAME.c_str() , SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, RESOLUTION_W, RESOLUTION_H, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	glViewport(0, 0, RESOLUTION_W, RESOLUTION_H);
	glMatrixMode(GL_MODELVIEW);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}
GLboolean GameEngine::ProcessEvents(){
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			return false;
		}

	}

	return true;
}
GLvoid GameEngine::Update(){
	glClear(GL_COLOR_BUFFER_BIT);
	time();

}
GLvoid GameEngine::Render(){


	glMatrixMode(GL_MODELVIEW);
	SDL_GL_SwapWindow(displayWindow);
}