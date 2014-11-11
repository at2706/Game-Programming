#pragma once
#include "Utilities.h"
#include "Vector.h"
class Particle
{
public:
	Particle();
	~Particle();

	Vector position;
	Vector velocity;
	float lifetime;
	float rotation;
	float sizeDeviation;
	Color colorDeviation;
	//lerp from start to end color
};

