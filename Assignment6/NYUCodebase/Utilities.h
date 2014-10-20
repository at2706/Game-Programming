#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <math.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

#define PI 3.14159265359f

#define GAME_NAME "Platformer"
#define RESOLUTION_W 960
#define RESOLUTION_H 600

#define ASPECT_RATIO_X 1.6f
#define ASPECT_RATIO_Y 1.0f
#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6

#define LEVEL_HEIGHT 22
#define LEVEL_WIDTH 32
#define TILE_SIZE 0.12f
#define SPRITE_COUNT_X 16
#define SPRITE_COUNT_Y 8