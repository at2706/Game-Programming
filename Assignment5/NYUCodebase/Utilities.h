#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <math.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <algorithm>

#define PI 3.14159265359f

#define GAME_NAME "Platformer"
#define RESOLUTION_W 960
#define RESOLUTION_H 600

#define ASPECT_RATIO_X 1.6f
#define ASPECT_RATIO_Y 1.0f
#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6