#include "ParticleEmitter.h"


ParticleEmitter::ParticleEmitter()
{
}


ParticleEmitter::~ParticleEmitter()
{
}


void ParticleEmitter::update(float elapsed){

}

void ParticleEmitter::render(){
	vector<GLfloat> particleVertices;
	vector<GLfloat> particleUVs;
	vector<GLuint> indices;
	for (int i = 0; i < particles.size(); i++) {
		float relativeLifetime = (particles[i].lifetime / maxLifetime);
		float size = lerp(startSize, endSize, relativeLifetime) + particles[i].sizeDeviation;
		GLuint vertexOffset = particleVertices.size() / 2;
		particleVertices.insert(particleVertices.end(), {
			particles[i].position.x - size, particles[i].position.y + size,
			particles[i].position.x - size, particles[i].position.y - size,
			particles[i].position.x + size, particles[i].position.y - size,
			particles[i].position.x + size, particles[i].position.y + size
		});
		particleUVs.insert(particleUVs.end(), {
			0.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f
		});
		indices.insert(indices.end(), { vertexOffset + 0, vertexOffset + 1, vertexOffset + 2, vertexOffset + 0, vertexOffset + 2,
			vertexOffset + 3 });
	}
	glTexCoordPointer(2, GL_FLOAT, 0, particleUVs.data());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());
}