#include "GameEngine.h"

GameEngine::GameEngine(){
	Setup();

	charSheet = new SpriteSheet("sprites.png");
	fontTexture = loadTexture("font1.png");
	bgTexture = loadTexture("grass.png");
	state = STATE_MAIN_MENU;

	GLfloat posX = -0.88f;
	GLfloat posY = 0.95f;
	GLint u = 0;
	alienDirection = 0.0f;
	movingRight = true;
	alienSpeed = 0.3f;

	alienFireDelay = 0.5f;
	alienLaserIndex = 0;
	alienFiringIndex = 5;

	bulletIndex = 0;
	spaceship = new Entity(charSheet, 0, 96 / 256.0f, 0.0f, -0.9f,
		51.0f / 128.0f, 31.0f / 256.0f, 0.0, 2.0f, 0.25f);

	startButton = new Entity(charSheet, 0, 0, 0.0f, 0.0f,
		81.0f / 128.0f, 64.0f / 256.0f, 0.0, 2.0f, 0.25f);


	for (GLint c = 0; c < ALIEN_COL; c++){
		for (GLint r = 0; r < ALIEN_ROW; r++){
			entities.push_back(new Entity(charSheet, u/ 128.0f, 129 / 256.0f, posX, posY,
							49.0f / 128.0f, 32.0f / 256.0f, 0.0, alienSpeed, 0.15f));
			posX += 1.2f / ALIEN_ROW;
		}
		u = 51;
		posY -= 1.5f / ALIEN_ROW;
		posX = -0.88f;
	}

}


GameEngine::~GameEngine(){
	SDL_Quit();
}



GLboolean GameEngine::ProcessEvents(){
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			return false;
		}

	}
	if (event.type == SDL_MOUSEBUTTONDOWN && state == STATE_MAIN_MENU){
		if (startButton->collision(event.button.x, event.button.y))
			state = STATE_GAME_LEVEL;
	}
	// Keyboard Events
	if (keys[SDL_SCANCODE_RIGHT] && spaceship->x < 0.85f) {
		spaceship->move(0.0f, elapsed);
	}
	else if (keys[SDL_SCANCODE_LEFT] && spaceship->x > -0.85f) {
		spaceship->move(180.0f, elapsed);
	}

	if (keys[SDL_SCANCODE_SPACE]) {
		if (lastBullet > BULLET_DELAY){
			bullets[bulletIndex] = new Entity(charSheet, 66 / 128.0f, 66 / 256.0f, spaceship->x, spaceship->y,
				13.0f / 128.0f, 32.0f / 256.0f, 0.0, BULLET_SPEED, 0.1f);
			lastBullet = 0.0f;
			if (bulletIndex < MAX_BULLETS) bulletIndex++;
			else bulletIndex = 0;
		}
	}
	lastBullet += elapsed;
	return true;
}
GLvoid GameEngine::Update(){
	glClear(GL_COLOR_BUFFER_BIT);
	time();

	switch (state){
	case STATE_MAIN_MENU:
		for (int i = 0; i < MAX_BULLETS; i++) {

			if (bullets[i] != NULL){
				bullets[i]->move(90.0f, elapsed);
				if (bullets[i]->y > 1.2f){
					delete bullets[i];
					bullets[i] = NULL;
				}
				else if (bullets[i]->collision(startButton)){
					delete bullets[i];
					bullets[i] = NULL;
					state = STATE_GAME_LEVEL;
				}
			}
		}
		break;
	case STATE_GAME_LEVEL:
		if (alienLastLaser > alienFireDelay){
			lasers[alienLaserIndex] = new Entity(charSheet, 66 / 128.0f, 66 / 256.0f, entities[alienFiringIndex]->x, entities[alienFiringIndex]->y,
				13.0f / 128.0f, 32.0f / 256.0f, 0.0, BULLET_SPEED, 0.1f);
			alienLastLaser = 0.0f;
			alienFiringIndex = rand() % entities.size();
			if (alienLaserIndex < MAX_LASERS) alienLaserIndex++;
			else alienLaserIndex = 0;
		}
		alienLastLaser += elapsed;

		if (alienRowCount != ROW_MAX * 2)
		{
			if ((alienDirection == 0.0f || alienDirection == 180.0f) && alienTravelTime >= 0.6f / alienSpeed){
				alienDirection = 270.0f;
				alienTravelTime = 0.0f;
			}

			else if (alienDirection == 270.0f && alienTravelTime >= 0.08f / alienSpeed){
				if (movingRight){
					alienDirection = 180.0f;
					movingRight = false;
				}
				else {
					alienDirection = 0.0f;
					movingRight = true;
				}
				alienTravelTime = 0.0f;
				alienRowCount++;
			}
		}

		else {
			if (movingRight && alienTravelTime >= 0.6f / alienSpeed){
				alienDirection = 180.0f;
				movingRight = false;
				alienTravelTime = 0.0f;
			}
			else if (!movingRight && alienTravelTime >= 0.6f / alienSpeed){
				alienDirection = 0.0f;
				movingRight = true;
				alienTravelTime = 0.0f;
			}

			if (alienSpeed < 1.2f) alienSpeed += 0.05f * elapsed;
		}

		alienTravelTime += elapsed;
		for (vector<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it) {
			(*it)->speed = alienSpeed;
			(*it)->move(alienDirection, elapsed);
		}

		for (int i = 0; i < MAX_LASERS; i++) {

			if (lasers[i] != NULL){
				lasers[i]->move(270.0f, elapsed);
				if (lasers[i]->y < -1.2f){
					delete lasers[i];
					lasers[i] = NULL;
				}
				if (lasers[i] != NULL && spaceship->collision(lasers[i])){
					state = STATE_GAME_OVER;
				}
			}
		}

		for (int i = 0; i < MAX_BULLETS; i++) {

			if (bullets[i] != NULL){
				bullets[i]->move(90.0f, elapsed);
				if (bullets[i]->y > 1.2f){
					delete bullets[i];
					bullets[i] = NULL;
				}

				for (vector<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it) {
					if (*it != nullptr && !(*it)->dead && bullets[i] != NULL && (*it)->collision(bullets[i])){
						delete bullets[i];
						bullets[i] = NULL;
						(*it)->dead = true;
						score++;

						if (score == (ALIEN_COL * ALIEN_ROW)) state = STATE_GAME_OVER;
					}
				}
			}
		}
		break;
	case STATE_GAME_OVER:

		break;
	}
}
GLvoid GameEngine::Render(){
	DrawBackground(spaceship->x * 0.07f);
	if (state != STATE_GAME_OVER)
	{
		spaceship->draw();
		for (int i = 0; i < MAX_BULLETS; i++) {
			if (bullets[i] != NULL) bullets[i]->draw();
		}
	}
	switch (state){
	case STATE_MAIN_MENU:
		startButton->draw();
		DrawText("START", 0.1f, -0.05f, 0.0f, 0.0f, 0.0f, 1.0f, 01.5f, 1.0f);
		
		break;
	case STATE_GAME_LEVEL:
		DrawText("SCORE:", 0.1f, -0.05f, -0.80f, 0.92f, 1.0f, 1.0f, 1.0f, 1.0f);
		DrawText(to_string(score), 0.1f, -0.05f, -0.6f, 0.92f, 1.0f, 1.0f, 1.0f, 1.0f);

		for (vector<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it) {
			if (!(*it)->dead) (*it)->draw();
		}

		for (int i = 0; i < MAX_LASERS; i++) {
			if (lasers[i] != NULL) lasers[i]->draw();
		}


		break;

	case STATE_GAME_OVER:
		DrawText("GAME OVER", 0.2f, -0.05f, 0.0f, 0.4f, 0.0f, 1.0f, 01.5f, 1.0f);
		DrawText("SCORE:", 0.1f, -0.05f, -0.2f, 0.1f, 1.0f, 1.0f, 1.0f, 1.0f);
		DrawText(to_string(score), 0.1f, -0.05f, 0.1f, 0.1f, 1.0f, 1.0f, 1.0f, 1.0f);

		break;
	}




	glMatrixMode(GL_MODELVIEW);
	SDL_GL_SwapWindow(displayWindow);
}

GLvoid GameEngine::time(){
	GLfloat ticks = (GLfloat)SDL_GetTicks() / 1000.0f;
	elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;
}
GLvoid GameEngine::Setup(){
	//DO NOT TOUCH
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow(GAME_NAME.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, RESOLUTION_W, RESOLUTION_H, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	glViewport(0, 0, RESOLUTION_W, RESOLUTION_H);
	glMatrixMode(GL_MODELVIEW);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

GLuint GameEngine::loadTexture(const char *image_path, GLint param) {
	SDL_Surface *surface = IMG_Load(image_path);
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	if (surface != NULL) glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param);
	SDL_FreeSurface(surface);
	return textureID;
}
GLvoid GameEngine::DrawText(string text, GLfloat size, GLfloat spacing, GLfloat x, GLfloat y, GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, fontTexture);
	glEnable(GL_BLEND);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x, y, 0.0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	GLfloat texture_size = 1.0 / 16.0f;
	vector<GLfloat> vertexData;
	vector<GLfloat> texCoordData;
	vector<GLfloat> colorData;

	for (GLint i = 0, j = -((GLfloat)text.size() / 2.0f); i < text.size(); i++, j++) {
		GLfloat texture_x = (GLfloat)(((GLint)text[i]) % 16) / 16.0f;
		GLfloat texture_y = (GLfloat)(((GLint)text[i]) / 16) / 16.0f;
		colorData.insert(colorData.end(), { r, g, b, a, r, g, b, a, r, g, b, a, r, g, b, a });
		vertexData.insert(vertexData.end(), { ((size + spacing) * j) + (-0.5f * size), 0.5f * size, ((size + spacing) * j) +
			(-0.5f * size), -0.5f * size, ((size + spacing) * j) + (0.5f * size), -0.5f * size, ((size + spacing) * j) + (0.5f * size), 0.5f
			* size });
		texCoordData.insert(texCoordData.end(), { texture_x, texture_y, texture_x, texture_y + texture_size, texture_x +
			texture_size, texture_y + texture_size, texture_x + texture_size, texture_y });
	}

	glColorPointer(4, GL_FLOAT, 0, colorData.data());
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glDrawArrays(GL_QUADS, 0, text.size() * 4);
	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

GLvoid GameEngine::DrawBackground(GLfloat offsetX){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, bgTexture);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	GLfloat quad[] = { -2.0f + offsetX, 2.0f, -2.0f + offsetX, -2.0f, 2.0f + offsetX, -2.0f, 2.0f + offsetX, 2.0f };
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);
	GLfloat quadUVs[] = { 0.0, 0.0, 0.0, 4.0, 4.0, 4.0, 4.0, 0.0 };
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
}