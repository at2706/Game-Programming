#include "Entity.h"

SDL_Window* displayWindow;
const char GAME_NAME[] = "Classic Pong";
const GLint RESOLUTION_W = 800;
const GLint RESOLUTION_H = 600;
const Uint8 *keys = SDL_GetKeyboardState(NULL);


class SheetSprite{
public:
	SheetSprite();
	SheetSprite(unsigned int textureID, float u, float v, float width, float height);
	float u;
	float v;
	float width;
	float height;

};

GLfloat time(){
	static GLfloat lastFrameTicks = 0.0f;
	static GLfloat elapsed = 0;

	float ticks = (float)SDL_GetTicks() / 1000.0f;
	elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	return elapsed;
}

GLuint LoadTexture(const char *image_path) {
	SDL_Surface *surface = IMG_Load(image_path);
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	if (surface != NULL) glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //Change from LINEAR to NEAREST
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //Change from LINEAR to NEAREST

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	SDL_FreeSurface(surface);
	return textureID;
}

void DrawSpritesheetSprite(int spriteTexture, int index, int spriteCountX, int spriteCountY){
	glBindTexture(GL_TEXTURE_2D, spriteTexture);

	float u = (float)(((int)index) % spriteCountX) / (float)spriteCountX;
	float v = (float)(((int)index) % spriteCountY) / (float)spriteCountY;
	float spriteWidth = 1.0f / (float)spriteCountX;
	float spriteHeight = 1.0f / (float)spriteCountY;
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

}

bool ProcessEvents(float elapsed){
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			return false;
		}
		//Keyboard Input
		
	}
	return true;
}

void Update(){
	glClear(GL_COLOR_BUFFER_BIT);
}

void Render(){
	
	glMatrixMode(GL_MODELVIEW);

	SDL_GL_SwapWindow(displayWindow);
}

void Cleanup(){
	SDL_Quit();
}

int main(int argc, char *argv[])
{
	
	Setup();

	while (ProcessEvents(time())) {
		Update();

		Render();
	}

	Cleanup();
	
	return 0;
}

/*
	vertex<float> vertexData;
	vertex<float> uvData;


	vertexData.insert(vertexData.end(),{((size+spacing) * i) + (-0.5f * size), (0.5f * size),
										(size * i) + (-0.5f * size), (-0.5f * size),
										(size * i) + (0.5f * size), (-0.5f * size),
										(size * i) + (0.5f * size), (0.5f * size)});

	uvData.insert(uvData.end(), {u, v, u, v+ spriteHeight, u+spriteWidth, v+spriteHeight, u+spriteWidth, v});

	glEnable(GL)BLEND);
	glBlendFunc(GL_SRC_ALPHA,

*/

/*
	SheetSprite mySprite = SheetSprite(spriteSheetTexture,425.0f/1024.0f, 468.0f/1024.0f,93.0f/1024.0f,84.0f/1024.0f);

	void Bullet::Update(float elapsed){
		GLfloat radian = (degree * PI) / 180.0f;

		x += cos(radian) * e * speed;
		y += sin(radian) * e * speed;

		timeAlive += elapsed;
	}

	void Bullet::Draw(){
		if(!visible){
			return;
		}

		GLfloat quad[] = {-size,size,

	}

	ClassDemoApp::shootBullet(){
		Bullet newBullet;
		newBullet.visible = true;
		newBullet.x = -1.2;
		newBullet.y = 0.0f;
		newBullet.angle = 0.0f;
		newBullet.size = 0.02;
		newBullet.speed = 0.0f;
	}

	#define FIXED_TIMESTEP 0.0166666f
	#define MAX_TIMESTEPS 6
	float timeLeftOver = 0.0f;

	float fixedElapsed = elapsed + timeLeftOver;
	

	if(fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS){
		fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
	}
	//update loop
	while(fixedElapsed >= FIXED_TIMESTEP){
		fixedElapsed -= FIXED_TIMESTAMP;
		FixedUpdate();
	}

	timeLeftOver = fixedElapsed;

	Update(elapsed);

	Entity::FixedUpdate(){
		
	}


	inside FixedUpdate()
	Friction / deceleration
					value,		target num, time
	velocity_x = lerp(velocity_x, 0.0f,elapsed * friction_x);
	controls
	velocity_x += gravity * FIXED_TIMESTEP;


	LinEar inteRPolation
	float lerp(float v0, float v1, float t){
		return (1.0-t)*v0+ t*v1;
	}


	penetration = fabs(disatance - radius1 - radius2); //float absolute value
	circle2.position -= penetration + offset;

	box-to-box
	solve each axis collision seperately

	Loop Entites(i){

	entities[i]->x += entities[i]->velocity_x * FIXED_TIMESTEP;

		loop through entitiess again(j):
			if(entities[j] != entities [i]{
				if(entities[i]->collidesWith(entities[j])){
					float yPenetration = fabs(entities[j]->x - entities[i]->x) - entities[i]->width*0.5f - entities[i]->width * 0.5f - entities[j]->width * 0.5f; //distance between y coords
				
					if(entities[j]->x > entities[i]->x) {
						entities[j]->x -=xPentration +0.001;
					}
					else entities[j]->x +=xPentration +0.001;

					entities[j]->y += yPenetration + 0.0001f;
				}
				entities[i]->velocity = 0.0f;
		}

	entities[i]->y += entities[i]->velocity_y * FIXED_TIMESTEP;
	//Same thing for y;
	}

	CLassDemoApp::FixedUpdate(){
		Entities Loop: entities[i] -> fixedUpdate();
		if(!entities[i]->isStatic){
			entities[i]
		}

		penetration distance * mass / total mass
		
*/