#include "GameEngine.h"

GameEngine::GameEngine(){
	Setup();
	elapsed = 0;
	gravity_x = 0.0f;
	gravity_y = -9.8f;
	tileSheet = loadTexture("arne_sprites.png", GL_NEAREST);
	UISheet = loadTexture("greenSheet.png", GL_NEAREST);
	fontTexture = loadTexture("font1.png", GL_NEAREST);
	bgTexture = loadTexture("colored_grass.png");
	state = STATE_MAIN_MENU;
	jumpSound = Mix_LoadWAV("jump.wav");

	sprite = new SpriteUniformed(tileSheet, 98, 16, 8);
	hero = new Entity(sprite);
	hero->setMovement(3.0f, 5.0f, 5.0f, 5.0f, 5.0f);

	sprite = new SheetSprite(UISheet, 190.0f / 512, 94.0f / 256, 100.0f / 512, 100.0f / 256);
	UImain = new UIElement(sprite, 0.0f,0.0f,3.0f,3.0f);

	sprite = new SheetSprite(UISheet, 380.0f / 512, 36.0f / 256, 38.0f / 512, 36.0f / 256);
	UIElement *ele = new UIElement(sprite,1.0f,1.0f);
	UImain->attach(ele);

	buildLevel();
}

GameEngine::~GameEngine(){
	Mix_FreeChunk(jumpSound);

	SDL_Quit();
}

GLboolean GameEngine::ProcessEvents(){
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			return false;
		}
	}
	 //Keyboard Events
	if (keys[SDL_SCANCODE_RIGHT]) {
		hero->isIdle = false;
		hero->facing = 0.0f;
	}
	else if (keys[SDL_SCANCODE_LEFT]) {
		hero->isIdle = false;
		hero->facing = 180.0f;
	}
	else{
		hero->isIdle = true;
	}

	if (keys[SDL_SCANCODE_UP] && hero->collidedBottom){
		Mix_PlayChannel(-1, jumpSound, 0);
		hero->velocity_y = 3.0f;
	}

	switch (state){
	case STATE_MAIN_MENU:
	
	case STATE_GAME_LEVEL:
	
	break;
	case STATE_GAME_OVER:
		
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
		
		break;
	case STATE_GAME_OVER:

		break;
	}
}
GLvoid GameEngine::Render(){
	DrawBackground();
	glLoadIdentity();

	GLfloat panX = (hero->x < MARGIN_LEFT) ? -MARGIN_LEFT : -hero->x;
	panX = (hero->x >(mapWidth * TILE_SIZE) - MARGIN_RIGHT) ? -mapWidth * TILE_SIZE + MARGIN_RIGHT : panX;
	GLfloat panY = (hero->y > -MARGIN_TOP) ? MARGIN_TOP : -hero->y;
	panY = (hero->y < -(mapHeight * TILE_SIZE) + MARGIN_BOTTOM) ? (mapHeight * TILE_SIZE) - MARGIN_BOTTOM : panY;
	
	glTranslatef(panX, panY, 0.0f);
	drawLevel();
	Entity::drawAll();

	glLoadIdentity();
	UImain->draw();
	switch (state){
	case STATE_MAIN_MENU:

		break;
	case STATE_GAME_LEVEL:

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
	if (type == "Start"){
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
	sprite = new SpriteUniformed(tileSheet, 3, 16, 8);
	for (GLfloat i = -(length / 2); i < (length / 2); i++){
		platform = new Entity(sprite, (i * sprite->width) + x, y);
		platform->isStatic = true;
	}
}