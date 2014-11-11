#include "GameEngine.h"

GameEngine::GameEngine(){
	Setup();
	elapsed = 0;
	gravity_x = 0.0f;
	gravity_y = -9.8f;
	tileSheet = loadTexture("sheet.png", GL_NEAREST);
	UISheet = loadTexture("greenSheet.png", GL_NEAREST);
	fontTexture = loadTexture("font1.png", GL_NEAREST);
	bgTexture = loadTexture("colored_grass.png");
	state = STATE_MAIN_MENU;
	jumpSound = Mix_LoadWAV("jump.wav");
	music = Mix_LoadMUS("music.mp3");
	Mix_PlayMusic(music, -1);
	Mix_VolumeMusic(20);
	SheetSprite *sprite;
	//<SubTexture name = "playerShip2_green.png" x = "112" y = "866" width = "112" height = "75" / >
	sprite = new SheetSprite(tileSheet, 112.0f / 1024, 866.0f / 1024, 112.0f / 1024, 75.0f / 1024);
	hero = new Entity(sprite);
	hero->setMovement(1.0f, 6.0f, 6.0f, 8.0f, 8.0f);
	hero->setPos(-ASPECT_RATIO_X + 0.1f,0.0f);
	hero->enableGravity = false;
	hero->rotation = -90.0f;
	hero->setScale(3.5f, 1.0f);

	//<SubTexture name="playerShip2_blue.png" x="112" y="791" width="112" height="75"/>
	sprite = new SheetSprite(tileSheet, 112.0f / 1024, 791.0f / 1024, 112.0f / 1024, 75.0f / 1024);
	hero2 = new Entity(sprite);
	hero2->setMovement(1.0f, 6.0f, 6.0f, 8.0f, 8.0f);
	hero2->setPos(ASPECT_RATIO_X - 0.1f, 0.0f);
	hero2->enableGravity = false;
	hero2->facing = 90.0f;
	hero2->setScale(3.5f, 1.0f);

	sprite = new SheetSprite(tileSheet, 651 / 1024.0f, 447 / 1024.0f, 43 / 1024.0f, 43 / 1024.0f);
	ball = new Entity(sprite);
	ball->enableGravity = false;
	ball->setMovement(5.0f, 0.01f, 0.01f, 0.0f, 0.0f);
	ball->facing = rand() % 30 + 40;
	ball->isIdle = false;
	ball->enableBounce = true;
	ball->setScale(2, 2);

	/*//<SubTexture name="green_panel.png" x="190" y="94" width="100" height="100"/>
	sprite = new SheetSprite(UISheet, 190.0f / 512, 94.0f / 256, 100.0f / 512, 100.0f / 256);
	UImain = new UIElement(sprite, -1.2f,0.4f,1.0f,1.0f);

	//<SubTexture name="green_boxCross.png" x="380" y="36" width="38" height="36"/>
	sprite = new SheetSprite(UISheet, 380.0f / 512, 36.0f / 256, 38.0f / 512, 36.0f / 256);
	UIElement *ele = new UIElement(sprite,0.0f,0.0f);
	UImain->attach(ele);*/

	screenShakeStart = 0.0f;
	screenShakeDuration = 0.5f;
}

GameEngine::~GameEngine(){
	Mix_FreeChunk(jumpSound);
	Mix_FreeMusic(music);

	SDL_Quit();
}

GLboolean GameEngine::ProcessEvents(){
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			return false;
		}
	}
	

	switch (state){
	case STATE_MAIN_MENU:
		if (keys[SDL_SCANCODE_S]){
			ball->setVelocity(0, 0);
			ball->isIdle = true;
			state = STATE_GAME_LEVEL;
		}
	break;
	case STATE_GAME_LEVEL:
		//Keyboard Events
		if (keys[SDL_SCANCODE_W]) {
			hero->facing = 90.0f;
			hero->rotation = -180.0f;
			hero->isIdle = false;
		}
		else if (keys[SDL_SCANCODE_S]) {
			hero->facing = 270.0f;
			hero->rotation = 0.0f;
			hero->isIdle = false;
		}
		else{
			hero->isIdle = true;
		}

		if (keys[SDL_SCANCODE_UP]) {
			hero2->facing = 90.0f;
			hero2->rotation = 0.0f;
			hero2->isIdle = false;
		}
		else if (keys[SDL_SCANCODE_DOWN]) {
			hero2->facing = 270.0f;
			hero2->rotation = -180.0f;
			hero2->isIdle = false;
		}
		else{
			hero2->isIdle = true;
		}

		if (keys[SDL_SCANCODE_SPACE] && ball->velocity_x == 0.0f) {
			ball->facing = rand() % 90 - 40;
			ball->setVelocity(2, 2);
			ball->isIdle = false;
		}
	break;
	case STATE_GAME_OVER:
		
	break;
	}

	return true;
}
GLvoid GameEngine::Update(){
	glClear(GL_COLOR_BUFFER_BIT);
	time();
	hero->isVisible = true;
	switch (state){
	case STATE_MAIN_MENU:
		
		break;
	case STATE_GAME_LEVEL:
		
		break;
	case STATE_GAME_OVER:

		break;
	}
}
GLvoid GameEngine::Render(){
	//DrawBackground();
	/*glLoadIdentity();

	GLfloat panX = (hero->x < MARGIN_LEFT) ? -MARGIN_LEFT : -hero->x;
	panX = (hero->x >(mapWidth * TILE_SIZE) - MARGIN_RIGHT) ? -mapWidth * TILE_SIZE + MARGIN_RIGHT : panX;
	GLfloat panY = (hero->y > -MARGIN_TOP) ? MARGIN_TOP : -hero->y;
	panY = (hero->y < -(mapHeight * TILE_SIZE) + MARGIN_BOTTOM) ? (mapHeight * TILE_SIZE) - MARGIN_BOTTOM : panY;
	
	glTranslatef(panX, panY, 0.0f);
	//drawLevel();*/

	GLfloat screenShakeSpeed = 15;
	GLfloat screenShakeIntensity = 0.04f;

	float animationAValue;
	float animationBValue;

	switch (state){
	case STATE_MAIN_MENU:
		animationAValue = mapValue(ticks, 1.5f, 2.0f, 0.0f, 1.0f);
		animationBValue = mapValue(ticks, 1.5f, 2.5f, 0.0f, 1.0f);

		DrawText("PONG", 0.2f, 0.0f, 0.0f, easeOutElastic(1.5f, 0.5f, animationBValue), 1.0f, 1.0f, 1.0f, 1.0f);
		DrawText("START", 0.1f, 0.0f, easeInOut(-1.6f, 0.0f, animationAValue), 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
	break;
	case STATE_GAME_LEVEL:

		screenShakeValue += elapsed;
		float animationAValue;
		if (screenShakeValue <= screenShakeDuration / 2)
 			animationAValue = mapValue(screenShakeValue, 0.0, screenShakeDuration / 2, 0.0f, 0.5f);
		else animationAValue = mapValue(screenShakeValue, screenShakeDuration, screenShakeDuration / 2, 0.5f, 0.0f);
		glTranslatef(sin(animationAValue * screenShakeSpeed) * screenShakeIntensity, 0.0f, 0.0f);
		Entity::drawAll();

		glLoadIdentity();
		drawBlackBox(0.8f);
		DrawText("SCORE", 0.1f, 0.0f, 0.0f, 0.9f, 1.0f, 1.0f, 1.0f, 1.0f);
		DrawText(to_string(score1), 0.1f, 0.0f, -0.65f, 0.9f, 1.0f, 1.0f, 1.0f, 1.0f);
		DrawText(to_string(score2), 0.1f, 0.0f, 0.65f, 0.9f, 1.0f, 1.0f, 1.0f, 1.0f);
	break;

	case STATE_GAME_OVER:


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
		Entity::fixedUpdateAll(this);
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
	glMatrixMode(GL_PROJECTION);
	glOrtho(-ASPECT_RATIO_X, ASPECT_RATIO_X, -ASPECT_RATIO_Y, ASPECT_RATIO_Y, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
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
	glPushMatrix();
	glTranslatef(x, y, 0.0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	GLfloat texture_size = 1.0 / 16.0f;
	vector<GLfloat> vertexData;
	vector<GLfloat> texCoordData;
	vector<GLfloat> colorData;

	for (GLfloat i = 0, j = - ((GLfloat)text.size() / 2); i < text.size(); i++, j++) {
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
	glPopMatrix();

}
GLvoid GameEngine::DrawBackground(GLfloat offsetX){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, bgTexture);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	GLfloat quad[] = {	-1.0f * ASPECT_RATIO_X + offsetX, 1.0f * ASPECT_RATIO_Y,
						-1.0f * ASPECT_RATIO_X + offsetX, -1.0f * ASPECT_RATIO_Y,
						1.0f * ASPECT_RATIO_X + offsetX, -1.0f * ASPECT_RATIO_Y,
						1.0f * ASPECT_RATIO_X + offsetX, 1.0f * ASPECT_RATIO_Y };
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

GLvoid GameEngine::buildLevel(){
	ifstream infile("LevelData.txt");
	string line;
	while (getline(infile, line)) {
			if (line == "[header]") {
			if (!readHeader(infile)) {
				return;
			}
			}
			else if (line == "[layer]") {
				readLayerData(infile);
			}
			else if (line == "[ObjectsLayer]") {
				readEntityData(infile);
			}
	}
}
GLvoid GameEngine::drawLevel(){
	vector<GLfloat> vertexData;
	vector<GLfloat> texCoordData;
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tileSheet);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef((-TILE_SIZE / 2), (-TILE_SIZE / 2), 0.0f);
	for (GLint y = 0; y < mapHeight; y++) {
		for (GLint x = 0; x < mapWidth; x++) {
			if (levelData[y][x] != 0) {
				GLfloat u = (GLfloat)(((GLint)levelData[y][x]) % SPRITE_COUNT_X) / (GLfloat)SPRITE_COUNT_X;
				GLfloat v = (GLfloat)(((GLint)levelData[y][x]) / SPRITE_COUNT_X) / (GLfloat)SPRITE_COUNT_Y;
				GLfloat spriteWidth = 1.0f / (GLfloat)SPRITE_COUNT_X;
				GLfloat spriteHeight = 1.0f / (GLfloat)SPRITE_COUNT_Y;
				vertexData.insert(vertexData.end(), {
					TILE_SIZE * x, -TILE_SIZE * y,
					TILE_SIZE * x, (-TILE_SIZE * y) - TILE_SIZE,
					(TILE_SIZE * x) + TILE_SIZE, (-TILE_SIZE * y) - TILE_SIZE,
					(TILE_SIZE * x) + TILE_SIZE, -TILE_SIZE * y });
					texCoordData.insert(texCoordData.end(), { u, v,
						u, v + (spriteHeight),
						u + spriteWidth, v + (spriteHeight),
						u + spriteWidth, v });
			}
		}
	}

	glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glDrawArrays(GL_QUADS, 0, vertexData.size() / 2);
	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glPopMatrix();
}

GLboolean GameEngine::readHeader(ifstream &stream){
	string line;
	mapWidth = -1;
	mapHeight = -1;
	while (getline(stream, line)) {
		if (line == "") { break; }
		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);
		if (key == "width") {
			mapWidth = atoi(value.c_str());
		}
		else if (key == "height"){
			mapHeight = atoi(value.c_str());
		}
	}
	if (mapWidth == -1 || mapHeight == -1) {
		return false;
	}
	else { // allocate our map data
		levelData = new unsigned char*[mapHeight];
		for (GLint i = 0; i < mapHeight; ++i) {
			levelData[i] = new unsigned char[mapWidth];
		}
		return true;
	}
}
GLboolean GameEngine::readLayerData(ifstream &stream){
	string line;
	while (getline(stream, line)) {
		if (line == "") { break; }
		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);
		if (key == "data") {
			for (GLint y = 0; y < mapHeight; y++) {
				getline(stream, line);
				istringstream lineStream(line);
				string tile;
				for (GLint x = 0; x < mapWidth; x++) {
					getline(lineStream, tile, ',');
					unsigned char val = (unsigned char)atoi(tile.c_str());
					if (val > 0) {
						// be careful, the tiles in this format are indexed from 1 not 0
						levelData[y][x] = val - 1;
					}
					else {
						levelData[y][x] = 0;
					}
				}
			}
		}
	}
	return true;
}
GLboolean GameEngine::readEntityData(ifstream &stream){
	string line;
	string type;
	while (getline(stream, line)) {
		if (line == "") { break; }
		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);
		if (key == "type") {
			type = value;
		}
		else if (key == "location") {
			istringstream lineStream(value);
			string xPosition, yPosition;
			getline(lineStream, xPosition, ',');
			getline(lineStream, yPosition, ',');
			GLfloat placeX = atoi(xPosition.c_str()) / 16 * TILE_SIZE;
			GLfloat placeY = atoi(yPosition.c_str()) / 16 * -TILE_SIZE;
			placeEntity(type, placeX, placeY);
		}
	}
	return true;
}

GLvoid GameEngine::placeEntity(string &type, GLfloat x, GLfloat y){
	if (type == "start"){
		hero->x = x;
		hero->y = y;
	}

	if(type == "platform"){
		drawPlatformHorizontal(4, x, y);
	}
}

GLboolean GameEngine::isSolidTile(unsigned char t){
	switch (t){
	case 1:
	case 2:
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
		return true;
	default: return false;
	}
	return false;
}

GLvoid GameEngine::drawPlatformHorizontal(GLfloat length, GLfloat x, GLfloat y){
	SheetSprite *sprite;
	Entity *platform;
	sprite = new SpriteUniformed(tileSheet, 3, 16, 8);
	for (GLfloat i = -(length / 2); i < (length / 2); i++){
		platform = new Entity(sprite, (i * sprite->width) + x, y);
		platform->isStatic = true;
	}
}

GLvoid GameEngine::drawBlackBox(float alpha) {
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_BLEND);


	glLoadIdentity();
	GLfloat quad3[] = { -1.6f, 1.0f, -1.6f, -1.0f, 1.6f, -1.0f, 1.6f, 1.0f };
	glVertexPointer(2, GL_FLOAT, 0, quad3);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat blackBox[] = { 0.0f, 0.0f, 0.0f, alpha, 0.0f, 0.0f, 0.0f, alpha, 0.0f, 0.0f, 0.0f, alpha, 0.0f, 0.0f, 0.0f, alpha };
	glColorPointer(4, GL_FLOAT, 0, blackBox);
	glEnableClientState(GL_COLOR_ARRAY);

	glDrawArrays(GL_QUADS, 0, 4);

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}