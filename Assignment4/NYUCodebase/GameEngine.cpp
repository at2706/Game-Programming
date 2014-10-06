#include "GameEngine.h"

GameEngine::GameEngine(){
	Setup();

	elapsed = 0;
	gravity_x = 0.0f;
	gravity_y = -1.0f;
	charSheet = loadTexture("sprites.png");
	fontTexture = loadTexture("font1.png", GL_NEAREST);
	bgTexture = loadTexture("colored_grass.png");
	reset();
	state = STATE_MAIN_MENU;

	drawPlatformHorizontal(18, 0.0f, -0.9f);
	drawPlatformHorizontal(8, -0.7f, -0.4f);
	drawPlatformHorizontal(8, 0.7f, -0.4f);
	drawPlatformHorizontal(18, 0.0f, 0.1f);
	drawPlatformHorizontal(5, -0.7f, 0.6f);
	drawPlatformHorizontal(5, 0.7f, 0.6f);

	drawPlatformVertical(18, 0.99f, 0.0f);
	drawPlatformVertical(18, -0.99f, 0.0f);
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
	// Keyboard Events
	switch (state){
	case STATE_MAIN_MENU:
		if (keys[SDL_SCANCODE_S]) state = STATE_GAME_LEVEL;
	case STATE_GAME_LEVEL:
	if (keys[SDL_SCANCODE_RIGHT] && hero->x < 0.8f) {
		hero->isIdle = false;
		hero->facing = 0.0f;
	}
	else if (keys[SDL_SCANCODE_LEFT] && hero->x > -0.8f) {
		hero->isIdle = false;
		hero->facing = 180.0f;
	}
	else{
		hero->isIdle = true;
	}

	if (keys[SDL_SCANCODE_X] && hero->collidedBottom) {
		hero->velocity_y = 1.2f;
	}
	if (keys[SDL_SCANCODE_C]) {
		if (lastBullet > BULLET_DELAY){
			bullets[bulletIndex]->isVisable = true;
			bullets[bulletIndex]->isIdle = false;
			bullets[bulletIndex]->enableCollisions = true;
			bullets[bulletIndex]->x = hero->x;
			bullets[bulletIndex]->y = hero->y;
			bullets[bulletIndex]->facing = hero->facing;
			bullets[bulletIndex]->velocity_x = hero->facing == 0.0f ? BULLET_SPEED : -BULLET_SPEED;
			lastBullet = 0.0f;
			if (bulletIndex < MAX_BULLETS - 1) bulletIndex++;
			else bulletIndex = 0;
		}
	}
	lastBullet += elapsed;
	break;
	case STATE_GAME_OVER:
		if (keys[SDL_SCANCODE_P]) reset();
	break;
	}

	return true;
}
GLvoid GameEngine::Update(){
	glClear(GL_COLOR_BUFFER_BIT);
	time();
	switch (state){
	case STATE_MAIN_MENU:
		
		break;
	case STATE_GAME_LEVEL:
		if (EnemyIndex != MAX_ENEMIES - 1){
			if (lastEnemy > ENEMY_DELAY){
				enemies[EnemyIndex]->isVisable = true;
				enemies[EnemyIndex]->isIdle = false;
				enemies[EnemyIndex]->enableCollisions = true;
				enemies[EnemyIndex]->x = -0.4f + (0.8f * (GLint)(rand() % 2));
				enemies[EnemyIndex]->y = 0.8f * (rand() % 3 + 2);
				enemies[EnemyIndex]->enableGravity = true;
				enemies[EnemyIndex]->facing = rand() % 2 + 1 == 1 ? 0.0f : 180.0f;

				lastEnemy = 0.0f;
				if (EnemyIndex < MAX_ENEMIES - 1) EnemyIndex++;
			}
		}
		lastEnemy += elapsed;
		for (Entity *enemy : enemies){
			if (!enemy->isVisable && lastEnemy > ENEMY_DELAY){
				enemies[EnemyIndex]->isVisable = true;
				enemies[EnemyIndex]->isIdle = false;
				enemies[EnemyIndex]->enableCollisions = true;
				enemies[EnemyIndex]->enableGravity = true;
				enemies[EnemyIndex]->facing = rand() % 2 + 1 == 1 ? 0.0f : 180.0f;
			}
			if (enemy->collidesWith(hero)) state = STATE_GAME_OVER;
			for (Entity *bullet : bullets){
				if (bullet->collidesWith(enemy)){
					bullet->isVisable = false;
					bullet->isIdle = true;
					bullet->enableCollisions = false;

					enemy->isVisable = false;
					enemy->isIdle = true;
					enemy->enableCollisions = false;
					enemy->x = -0.4f + (0.8f * (GLint)(rand() % 2));
					enemy->y = 0.8f * (rand() % 3 + 2);
					enemy->enableGravity = false;
					enemy->x = -0.4f + (0.8f * (GLint)(rand() % 2));
					enemy->y = 0.8f * (rand() % 3 + 2);

					score++;
				}
			}
		}

		if (hero->y < -1.2) state = STATE_GAME_OVER;
		break;
	case STATE_GAME_OVER:

		break;
	}
}
GLvoid GameEngine::Render(){
	DrawBackground();
	switch (state){
	case STATE_MAIN_MENU:
		DrawText("Press S to START", 0.1f, -0.05f, 0.0f, 0.0f, 0.0f, 1.0f, 01.5f, 1.0f);
		break;
	case STATE_GAME_LEVEL:
		Entity::drawAll();
		break;

	case STATE_GAME_OVER:
		string msg = "GAME OVER";

		DrawText(msg, 0.2f, -0.05f, 0.0f, 0.4f, 0.0f, 1.0f, 01.5f, 1.0f);
		DrawText("SCORE:", 0.1f, -0.05f, -0.2f, 0.1f, 1.0f, 1.0f, 1.0f, 1.0f);
		DrawText(to_string(score), 0.1f, -0.05f, 0.2f, 0.1f, 1.0f, 1.0f, 1.0f, 1.0f);

		DrawText("Press P to Play Again", 0.1f, -0.05f, 0.0f, -0.2f, 1.0f, 1.0f, 1.0f, 1.0f);

		break;
	}

	glMatrixMode(GL_MODELVIEW);
	SDL_GL_SwapWindow(displayWindow);
}

GLvoid GameEngine::time(){
	ticks = (GLfloat)SDL_GetTicks() / 1000.0f;
	elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	GLfloat fixedElapsed = elapsed + timeLeftOver;
	if (fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS) {
		fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
	}

	while (fixedElapsed >= FIXED_TIMESTEP) {
		fixedElapsed -= FIXED_TIMESTEP;
		Entity::fixedUpdateAll(gravity_x,gravity_y);
		collisionPen();
	}
	timeLeftOver = fixedElapsed;


}
GLvoid GameEngine::Setup(){
	//DO NOT TOUCH
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow(GAME_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, RESOLUTION_W, RESOLUTION_H, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	glViewport(0, 0, RESOLUTION_W, RESOLUTION_H);
	glMatrixMode(GL_MODELVIEW);
	glOrtho(-ASPECT_RATIO_X, ASPECT_RATIO_X, -ASPECT_RATIO_Y, ASPECT_RATIO_Y, -1.0, 1.0);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	elapsed = 0;
	lastFrameTicks = 0;
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

	for (GLfloat i = 0, j = - ((GLfloat)text.size() / 2.0f); i < text.size(); i++, j++) {
		GLfloat texture_x = (GLfloat)(((GLint)text[i]) % 16) / 16.0f;
		GLfloat texture_y = (GLfloat)(((GLint)text[i]) / 16) / 16.0f;
		colorData.insert(colorData.end(), { r, g, b, a, r, g, b, a, r, g, b, a, r, g, b, a });
		vertexData.insert(vertexData.end(), { ((size + spacing) * j) + (-0.5f * size), 0.5f * size,
											((size + spacing) * j) + (-0.5f * size), -0.5f * size ,
											((size + spacing) * j) + (0.5f * size), -0.5f * size,
											((size + spacing) * j) + (0.5f * size), 0.5f * size });
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
	GLfloat quad[] = { -1.0f + offsetX, 1.0f, -1.0f + offsetX, -1.0f, 1.0f + offsetX, -1.0f, 1.0f + offsetX, 1.0f };
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

GLvoid GameEngine::collisionPen(){
	vector<Entity*>::iterator end = Entity::entities.end();

	for (vector<Entity*>::iterator it = Entity::entities.begin(); it != end; ++it){
		if ((*it)->enableCollisions)
		for (vector<Entity*>::iterator it2 = Entity::entities.begin(); it2 != end; ++it2){
			if (it != it2 && (*it)->collidesWith((*it2)) && !(*it)->isStatic && (*it2)->isStatic && !(*it)->collidedLeft && !(*it)->collidedRight){
				GLfloat distance_y = fabs((*it2)->y - (*it)->y);
				GLfloat height1 = (*it)->sprite->height * 0.5f * (*it)->scale;
				GLfloat height2 = (*it2)->sprite->height * 0.5f * (*it2)->scale;
				GLfloat yPenetration = fabs(distance_y - height1 - height2);

				GLfloat distance_x = fabs((*it2)->x - (*it)->x);
				GLfloat width1 = (*it)->sprite->width * 0.5f * (*it)->scale;
				GLfloat width2 = (*it2)->sprite->width * 0.5f * (*it2)->scale;
				GLfloat xPenetration = fabs(distance_x - width1 - width2);

				if (yPenetration < xPenetration){
					if ((*it)->y < (*it2)->y) {
						(*it)->y -= yPenetration;
						(*it)->collidedTop = true;
					}
					else{
						(*it)->y += yPenetration;
						(*it)->collidedBottom = true;
					}


					if (!(*it2)->isStatic) (*it2)->y += yPenetration;
					(*it)->y += (*it)->velocity_y * FIXED_TIMESTEP;
					(*it)->velocity_y = (*it)->enableBounce ? fabs((*it)->velocity_y) : 0.0f;
				}

				

				else{
					if ((*it)->x < (*it2)->x) {
						(*it)->x -= xPenetration;
						(*it)->collidedRight = true;
					}
					else{
						(*it)->x += xPenetration;
						(*it)->collidedLeft = true;
					}


					if (!(*it2)->isStatic) (*it2)->x += xPenetration;
					(*it)->x += (*it)->velocity_x * FIXED_TIMESTEP;
					(*it)->velocity_x = 0.0f;
				}
			}
		}
	}
}

GLvoid GameEngine::drawPlatformHorizontal(GLfloat length, GLfloat x, GLfloat y){
	placeHolderSprite = new SheetSprite(charSheet, 0, 95 / 128.0f, 0.075f, 0.075f);
	for (GLfloat i = -(length / 2) + 0.3f; i < (length / 2); i++)
	{
		platform = new Entity(placeHolderSprite, (i * 0.075f) + x, y);
		platform->isStatic = true;
	}
}

GLvoid GameEngine::drawPlatformVertical(GLfloat length, GLfloat x, GLfloat y){
	placeHolderSprite = new SheetSprite(charSheet, 0, 72 / 128.0f, 0.075f, 0.075f);
	for (GLfloat i = -(length / 2) + 0.3f; i < (length / 2); i++)
	{
		platform = new Entity(placeHolderSprite, x, (i * 0.075f * ASPECT_RATIO_X) + y);
		platform->isStatic = true;
	}
}

GLvoid GameEngine::reset(){
	lastBullet = 0.0f;
	bulletIndex = 0;
	score = 0;
	lastEnemy = 0.0f;
	EnemyIndex = 0;
	placeHolderSprite = new SheetSprite(charSheet, 0, 0, 0.5, 0.5);
	hero = new Entity(placeHolderSprite, 0.0f, 0.0f, 0.6f, 0.1f, 0.1f);

	bulletSprite = new SheetSprite(charSheet, 23 / 256.0f, 93 / 128.0f, 19 / 256.0f, 19 / 128.0f);
	for (int i = 0; i < MAX_BULLETS; i++) {
		bullets[i] = new Entity(bulletSprite, hero->x, hero->y, BULLET_SPEED, 1.0, 0.2f);
		bullets[i]->enableGravity = false;
		bullets[i]->enableCollisions = false;
		bullets[i]->isVisable = false;
	}

	placeHolderSprite = new SheetSprite(charSheet, 25.0f / 256.0f, 72.0f / 128.0f, 19.0f / 256.0f, 19.0f / 128.0f);

	for (int i = 0; i < MAX_ENEMIES; i++) {
		
		enemies[i] = new Entity(placeHolderSprite, 0.0, 1.2f, 0.3f, 1.0f, 1.0f);
		enemies[i]->enableGravity = false;
		enemies[i]->enableCollisions = false;
		enemies[i]->isVisable = false;
	}
	state = STATE_GAME_LEVEL;

}