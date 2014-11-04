#include "Entity.h"
#include "GameEngine.h"

vector<Entity*> Entity::entities;
Entity::Entity(SheetSprite *sheet, GLfloat posX, GLfloat posY,
				GLfloat speed, GLfloat mass, GLfloat scale, GLfloat health)
				: scale_x(scale), scale_y(scale), health(health){
	sprite = sheet;
	setPos(posX, posY);
	setMovement(speed);
	setFacing(0.0);

	matrix = Matrix();

	velocity_x = 0;
	velocity_r = 0;
	isStatic = false;
	isIdle = true;
	isVisible = true;

	enableGravity = true;
	enableBounce = false;

	enableCollisions = true;
	collidedTop = false;
	collidedBottom = false;
	collidedLeft = false;
	collidedRight = false;

	entities.push_back(this);
	id = entities.end();
}

Entity::~Entity(){ entities.erase(id); }
GLvoid Entity::draw(){
	if (isVisible){
		buildMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glMultMatrixf(matrix.ml);

		sprite->draw(x, y, facing, scale_x, scale_y);

		glPopMatrix();
	}
}
GLvoid Entity::drawAll(){
	for (vector<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
		(*it)->draw();
}
GLfloat Entity::lerp(GLfloat v0, GLfloat v1, GLfloat t) {
	return (1.0f - t)*v0 + t*v1;
}
GLvoid Entity::worldToTileCoordinates(float worldX, float worldY, int *gridX, int *gridY) {
	*gridX = (int)((worldX) / TILE_SIZE) + sprite->width;
	*gridY = (int)((-worldY) / TILE_SIZE) + sprite->height;
}
//GLvoid setEdgeVectors(vector<Vector> &points, vector<Vector> &edges){
//	top = Vector(tr.x - tl.x, tr.y - tl.y, 0.0);
//	bot = Vector(br.x - bl.x, br.y - bl.y, 0.0);
//	right = Vector(tr.x - br.x, tr.y - br.y, 0.0);
//	left = Vector(tl.x - bl.x, tl.y - bl.y, 0.0);
//}

Vector getEdgeVector(Vector v1, Vector v2){
	return Vector(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z);
}

GLvoid Entity::fixedUpdate(GameEngine *g){
	collidedTop = false;
	collidedBottom = false;
	collidedLeft = false;
	collidedRight = false;
	msg1 = "false";
	msg2 = "false";
	if (!isStatic){

		if (rotating) moveR();
		else decelerateR();

		if (!isIdle){
			moveY();
			//collisionPenY();
			//tileCollisionY(g);
			collisionAxis();
			moveX();
			//collisionPenX();
			//tileCollisionX(g);
			collisionAxis();
		}
		else{
			decelerateY();
			//collisionPenY();
			//tileCollisionY(g);
			collisionAxis();
			decelerateX();
			//collisionPenX();
			//tileCollisionX(g);
			collisionAxis();
		}
		if (enableGravity && !collidedBottom){
			velocity_x += g->gravity_x * FIXED_TIMESTEP;
			velocity_y += g->gravity_y * FIXED_TIMESTEP;
		}

		//Screen Wrap
		if (x > ASPECT_RATIO_X + 0.1f) x = -ASPECT_RATIO_X - 0.05f;
		else if (x < -ASPECT_RATIO_X - 0.1f) x = ASPECT_RATIO_X + 0.05f;

		if (y > ASPECT_RATIO_Y + 0.1f) y = -ASPECT_RATIO_Y - 0.05f;
		else if (y < -ASPECT_RATIO_Y - 0.1f) y = ASPECT_RATIO_Y + 0.05f;

	}
}
GLvoid Entity::fixedUpdateAll(GameEngine *g){
	for (vector<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
		(*it)->fixedUpdate(g);
}

GLvoid Entity::setScale(GLfloat x, GLfloat y){
	scale_x = x;
	scale_y = y;
}
GLvoid Entity::setPos(GLfloat posX, GLfloat posY){
	x = posX;
	y = posY;
}
GLvoid Entity::setMovement(GLfloat s, GLfloat accelX, GLfloat accelY, GLfloat FricX, GLfloat FricY){
	speed = s;
	acceleration_x = accelX;
	acceleration_y = accelY;
	friction_x = FricX;
	friction_y = FricY;
}
GLvoid Entity::setFacing(GLfloat degree, GLfloat f){
	facing = degree;
	friction_r = f;
}
GLvoid Entity::setAnimation(GLuint *arr){
	animationFrames = arr;
}

GLvoid Entity::rotate(GLfloat degree){
	facing += degree;
	if (facing > 360.0f)	facing -= 360.0f;
	else if (facing < -360.0f) facing += 360.0f;
}
GLvoid Entity::moveX(){
	GLfloat radian = (facing * PI) / 180.0f;
	velocity_x += acceleration_x * FIXED_TIMESTEP * cos(radian);

	/*if (velocity_x > speed) velocity_x = speed;
	else if (velocity_x < -speed) velocity_x = -speed;*/
	x += velocity_x * FIXED_TIMESTEP;

}
GLvoid Entity::moveY(){
	GLfloat radian = (facing * PI) / 180.0f;
	velocity_y += acceleration_y * FIXED_TIMESTEP * sin(radian);

	/*if (velocity_y > speed) velocity_y = speed;
	else if (velocity_y < -speed) velocity_y = -speed;*/
	y += velocity_y * FIXED_TIMESTEP;
}
GLvoid Entity::decelerateX(){
	velocity_x = lerp(velocity_x, 0.0f, FIXED_TIMESTEP * friction_x);
	x += velocity_x * FIXED_TIMESTEP;
}
GLvoid Entity::decelerateY(){
	velocity_y = lerp(velocity_y, 0.0f, FIXED_TIMESTEP * friction_y);
	y += velocity_y * FIXED_TIMESTEP;
}

GLvoid Entity::moveR(){
	velocity_r += acceleration_r * FIXED_TIMESTEP;
	rotate(velocity_r * FIXED_TIMESTEP);
}
GLvoid Entity::decelerateR(){
	velocity_r = lerp(velocity_r, 0.0f, FIXED_TIMESTEP * friction_r);
	rotate(velocity_r * FIXED_TIMESTEP);
}

//Box-box Collision
GLboolean Entity::collidesWith(Entity *e){
	if (!enableCollisions || !e->enableCollisions) return false;
	GLfloat top = y + ((sprite->height * scale_y) / 2);
	GLfloat bot = y - ((sprite->height * scale_y) / 2);
	GLfloat left = x - ((sprite->width * scale_x) / 2);
	GLfloat right = x + ((sprite->width * scale_x) / 2);

	GLfloat etop = e->y + ((e->sprite->height * e->scale_y) / 2);
	GLfloat ebot = e->y - ((e->sprite->height * e->scale_y) / 2);
	GLfloat eleft = e->x - ((e->sprite->width * e->scale_x) / 2);
	GLfloat eright = e->x + ((e->sprite->width * e->scale_x) / 2);

	return !(bot > etop || top < ebot || left > eright || right < eleft);
}
//Box-point Collision
GLboolean Entity::collidesWith(GLfloat posX, GLfloat posY){
	if (!enableCollisions) return false;
	GLfloat top = y + ((sprite->height * scale_y) / 2);
	GLfloat bot = y - ((sprite->height * scale_y) / 2);
	GLfloat left = x - ((sprite->width * scale_x) / 2);
	GLfloat right = x + ((sprite->width * scale_x) / 2);

	return !((posX > left && posX < right) || (posY > bot && posY < top));
}
//Non-axis aligned bounding boxes
GLboolean Entity::collisionCheck(Entity *e){
	if (!enableCollisions || !e->enableCollisions) return false;
	buildMatrix();
	e->buildMatrix();

	//Matrix entity1Inverse = matrix.inverse();
	//Matrix entity2Inverse = e->matrix.inverse();

	vector<Vector> points;
	points.push_back(Vector(-e->sprite->width / 2, e->sprite->height / 2, 0.0f));
	points.push_back(Vector(-e->sprite->width / 2, -e->sprite->height / 2, 0.0f));
	points.push_back(Vector(e->sprite->width / 2, e->sprite->height / 2, 0.0f));
	points.push_back(Vector(e->sprite->width / 2, -e->sprite->height / 2, 0.0f));

	for (int i = 0; i < 4; i++){
		points[i] = matrix * points[i];
	}

	vector<Vector> edges;
	for (int i = 0; i < points.size(); i++){
		if (i == points.size() - 1) edges.push_back(getEdgeVector(points[0], points[i]));
		else edges.push_back(getEdgeVector(points[i+1], points[i]));
	}

	vector<Vector> epoints;
	epoints.push_back(Vector(-e->sprite->width / 2, e->sprite->height / 2, 0.0f));
	epoints.push_back(Vector(-e->sprite->width / 2, -e->sprite->height / 2, 0.0f));
	epoints.push_back(Vector(e->sprite->width / 2, e->sprite->height / 2, 0.0f));
	epoints.push_back(Vector(e->sprite->width / 2, -e->sprite->height / 2, 0.0f));

	for (int i = 0; i < 4; i++){
		epoints[i] = e->matrix * epoints[i];
		epoints[i].normalize();
	}

	for (int i = 0; i < epoints.size(); i++){
		if (i == points.size() - 1) edges.push_back(getEdgeVector(epoints[0], epoints[i]));
		else edges.push_back(getEdgeVector(epoints[i + 1], epoints[i]));
	}

	vector<float> penetrations;
	//Axis Loop A
	for (int i = 0; i < edges.size(); i++){
		edges[i].normalize();
		Vector axis = edges[i];

		//Projection Loop for A
		vector<float> projectionsA;
		for (int j = 0; j < 4; j++){
			projectionsA.push_back(axis.dotProduct(points[j]));
		}

		sort(projectionsA.begin(), projectionsA.end());
		float aMin = projectionsA.front(), aMax = projectionsA.back();

		//Projection Loop for B
		vector<float> projectionsB;
		for (int j = 0; j < 4; j++){
			projectionsB.push_back(axis.dotProduct(points[j]));
		}

		sort(projectionsB.begin(), projectionsB.end());
		float bMin = projectionsB.front(), bMax = projectionsB.back();

		if (aMin <= bMax || aMax >= bMin){
			penetrations.push_back(min(aMax - bMin, bMax - aMin));
		}
		else
			return false;
	}

	//sort(penetrations.front(), penetrations.back());

	/*
	get all corners for A
	get edges and normalize

	vector of penetrations here;
	Loop through axes{
		project all of both corners on to the current axis
		find the min and max of both shapes

		if they are colliding,
			then calculate the pentration,
			multiply it back with the edge,
			and store it in the vector
		else it's not colliding, then return false and stop the function;
	}
	find the smallest penetration in the vector
	move x and y with that vector
	*/

	/*Vector tl = Vector(-e->sprite->width, e->sprite->height, 0.0f);
	Vector bl = Vector(-e->sprite->width, -e->sprite->height, 0.0f);
	Vector tr = Vector(e->sprite->width, e->sprite->height, 0.0f);
	Vector br = Vector(e->sprite->width, -e->sprite->height, 0.0f);

	Vector top, right, left, bot;
	setEdgeVectors(tl, bl, tr, br, top, right, left, bot);

	top.normalize();
	right.normalize();
	left.normalize();
	bot.normalize();

	Vector ent2TL = Vector(-e->sprite->width, e->sprite->height, 0.0f);
	Vector ent2BL = Vector(-e->sprite->width, -e->sprite->height, 0.0f);
	Vector ent2TR = Vector(e->sprite->width, e->sprite->height, 0.0f);
	Vector ent2BR = Vector(e->sprite->width, -e->sprite->height, 0.0f);

	Vector e2top, e2right, e2left, e2bottom;
	setEdgeVectors(ent2TL, ent2BL, ent2TR, ent2BR, e2top, e2right, e2left, e2bottom);

	e2top.normalize();
	e2right.normalize();
	e2left.normalize();
	e2bottom.normalize();

	Vector axis = right;

	float BRProjected = axis * br;
	float TRProjected = axis * tr;

	float minX = min(BRProjected, TRProjected);
	float maxX = min(BRProjected, TRProjected);
	float aw = maxX - minX;
	
	float ent2TLprojected = axis * ent2TL;
	float ent2BLprojected = axis * ent2BL;
	float ent2TRprojected = axis * ent2TR;
	float ent2BRprojected = axis * ent2TR;

	minX = min(min(min(ent2TLprojected, ent2BLprojected), ent2TRprojected), ent2BRprojected);
	maxX = max(max(max(ent2TLprojected, ent2BLprojected), ent2TRprojected), ent2BRprojected);
	float bw = maxX - minX;

	float penX = fabs(x - e->x) - ((aw + bw));

	axis = bot;

	float BLProjected = axis * bl;
	BRProjected = axis * br;

	float minY = min(BLProjected, BRProjected);
	float maxY = min(BLProjected, BRProjected);
	float ah = maxY - minY;

	ent2TLprojected = axis * ent2TL;
	ent2BLprojected = axis * ent2BL;
	ent2TRprojected = axis * ent2TR;
	ent2BRprojected = axis * ent2TR;

	minY = min(min(min(ent2TLprojected, ent2BLprojected), ent2TRprojected), ent2BRprojected);
	maxY = max(max(max(ent2TLprojected, ent2BLprojected), ent2TRprojected), ent2BRprojected);
	float bh = maxY - minY;
	
	float penY = fabs(y - e->y) - ((ah + bh));

	if (penY < 0 && penX < 0){
		y += (penY - 0.0001f) /2;

		x += (penX - 0.0001f) /2;

		if (enableBounce){
			velocity_y = -velocity_y;
			velocity_x = -velocity_x;
			rotate(180);
		}
		else{
			velocity_x = 0.0f;
			velocity_y = 0.0f;
		}

		msg1 = "trueY: " + to_string(penY) + " / " + to_string(e->y);
		msg2 = "trueX: " + to_string(penX) + " / " + to_string(e->x);
	}
	*/
	/*//transform to world coordinates
	ent2TL = e->matrix * ent2TL;
	ent2BL = e->matrix * ent2BL;
	ent2TR = e->matrix * ent2TR;
	ent2BR = e->matrix * ent2BR;

	ent2TL = entity1Inverse * ent2TL;
	ent2BL = entity1Inverse * ent2BL;
	ent2TR = entity1Inverse * ent2TR;
	ent2BR = entity1Inverse * ent2BR;

	//check x axis
	float minX = min(min(min(ent2TL.x, ent2BL.x), ent2TR.x), ent2BR.x);
	float maxX = max(max(max(ent2TL.x, ent2BL.x), ent2TR.x), ent2BR.x);

	if (!(minX <= sprite->width && maxX >= -sprite->width)){
		return false;
	}

	//check y axis
	float minY = min(min(min(ent2TL.y, ent2BL.y), ent2TR.y), ent2BR.y);
	float maxY = max(max(max(ent2TL.y, ent2BL.y), ent2TR.y), ent2BR.y);

	if (!(minY <= sprite->height && maxY >= -sprite->height)){
		return false;
	}
	GLfloat radian = (facing * PI) / 180.0f;

	float penY = min(maxY - sprite->height, sprite->height - maxY) * -sin(radian) + 0.0001f;
	y += penY;

	float penX = min(maxX - sprite->width, sprite->width - maxX) * -cos(radian) + 0.0001f;
	x += penX;

	velocity_x = 0;
	velocity_y = 0;*/
	msg1 = "TRUE";
	return true;
}

GLvoid Entity::collisionPenY(){
	vector<Entity*>::iterator end = entities.end();
	for (vector<Entity*>::iterator it2 = entities.begin(); it2 != end; ++it2){
		if (this != (*it2) && (*it2)->isStatic && collidesWith((*it2))){
			GLfloat distance_y = fabs((*it2)->y - y);
			GLfloat height1 = sprite->height * 0.5f * scale_y;
			GLfloat height2 = (*it2)->sprite->height * 0.5f * (*it2)->scale_y;
			GLfloat yPenetration = fabs(distance_y - height1 - height2);

			if (y > (*it2)->y){
				y += yPenetration + 0.0001f;
				collidedBottom = true;
			}
			else{
				y -= yPenetration + 0.0001f;
				collidedTop = true;
			}

			//if (!(*it2)->isStatic) (*it2)->y += yPenetration;
			velocity_y = 0.0f;
		}
	}
}
GLvoid Entity::collisionPenX(){
	vector<Entity*>::iterator end = entities.end();
	for (vector<Entity*>::iterator it2 = entities.begin(); it2 != end; ++it2){
		if (this != (*it2) && (*it2)->isStatic && collidesWith((*it2))){
			GLfloat distance_x = fabs((*it2)->x - x);
			GLfloat width1 = sprite->width * 0.5f * scale_x;
			GLfloat width2 = (*it2)->sprite->width * 0.5f * (*it2)->scale_x;
			GLfloat xPenetration = fabs(distance_x - width1 - width2);

			if (x > (*it2)->x){
				x += xPenetration + 0.0001f;
				collidedRight = true;
			}
			else{
				x -= xPenetration + 0.0001f;
				collidedLeft = true;
			}

			velocity_x = 0.0f;
		}
	}
}

GLvoid Entity::tileCollisionY(GameEngine *g){
	if (gridY < 0 || gridY > g->mapHeight) { return; }

	GLint *gridXptr = &gridX, *gridYptr = &gridY;
	worldToTileCoordinates(x, y, gridXptr, gridYptr);

	GLint tileBot = g->levelData[gridY][gridX];
	GLint tileBotRight = g->levelData[gridY][gridX + 1];
	GLint tileTop = g->levelData[gridY - 1][gridX];

	if ((g->isSolidTile(tileBot) || g->isSolidTile(tileBotRight)) && velocity_y < 0.0f){
		GLfloat distance_y = fabs(((gridY - 1) * TILE_SIZE) + y);
		GLfloat height1 = sprite->height * 0.5f * scale_y;
		GLfloat height2 = TILE_SIZE * 0.5f;
		GLfloat yPenetration = fabs(distance_y - height1 - height2);

		y += yPenetration + 0.0001f;
		collidedBottom = true;

		velocity_y = 0.0f;
	}

	if (g->isSolidTile(tileTop) && velocity_y > 0.0f){
		GLfloat distance_y = fabs(((gridY)* TILE_SIZE) + y);
		GLfloat height1 = sprite->height * 0.5f * scale_y;
		GLfloat height2 = TILE_SIZE * 0.5f;
		GLfloat yPenetration = fabs(distance_y - height1 - height2);

		y -= yPenetration - 0.0001f;

		velocity_y = 0.0f;
	}
}
GLvoid Entity::tileCollisionX(GameEngine *g){
	GLint *gridXptr = &gridX, *gridYptr = &gridY;
	worldToTileCoordinates(x, y, gridXptr, gridYptr);

	GLint tileLeft = g->levelData[gridY][gridX];
	GLint tileRight = g->levelData[gridY][gridX + 1];

	if (g->isSolidTile(tileLeft) && velocity_x < 0.0f){
		GLfloat distance_x = fabs(((gridX) * TILE_SIZE) - x);
		GLfloat width1 = sprite->width * 0.5f * scale_x;
		GLfloat width2 = TILE_SIZE * 0.5f;
		GLfloat xPenetration = fabs(distance_x - width1 - width2);

		x += xPenetration + 0.0001f;
		collidedLeft = true;

		velocity_x = 0.0f;
	}

	if (g->isSolidTile(tileRight) && velocity_x > 0.0f){
		GLfloat distance_x = fabs(((gridX + 1)* TILE_SIZE) - x);
		GLfloat width1 = sprite->width * 0.5f * scale_x;
		GLfloat width2 = TILE_SIZE * 0.5f;
		GLfloat xPenetration = fabs(distance_x - width1 - width2);

		x -= xPenetration - 0.0001f;
		collidedLeft = true;

		velocity_x = 0.0f;
	}
}

GLvoid Entity::collisionAxis(){
	vector<Entity*>::iterator end = entities.end();
	for (vector<Entity*>::iterator it2 = entities.begin(); it2 != end; ++it2){
		if (this != (*it2) && collisionCheck((*it2))){
			
		}
	}
}

GLvoid Entity::buildMatrix(){
	Matrix scaleMatrix;
	scaleMatrix.m[0][0] = scale_x;
	scaleMatrix.m[1][1] = scale_y;

	Matrix translateMatrix;
	translateMatrix.m[3][0] = x;
	translateMatrix.m[3][1] = y;

	GLfloat radian = ((rotation + facing) * PI) / 180.0f; //Rotates with Facing
	Matrix rotationMatrix;
	rotationMatrix.m[0][0] = cos(radian);
	rotationMatrix.m[1][0] = -sin(radian);
	rotationMatrix.m[0][1] = sin(radian);
	rotationMatrix.m[1][1] = cos(radian);

	matrix = scaleMatrix * rotationMatrix * translateMatrix;
}