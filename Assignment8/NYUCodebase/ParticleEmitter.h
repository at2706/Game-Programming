#pragma once
#include "Utilities.h"
#include "Particle.h"
class ParticleEmitter
{
public:
	ParticleEmitter();
	~ParticleEmitter();

	void update(float elapsed);
	void render();

	Vector position;
	Vector gravity;
	float maxLifetime;

	float startSize;
	float endSize;
	float sizeDeviation;

	vector<Particle> particles;
};
