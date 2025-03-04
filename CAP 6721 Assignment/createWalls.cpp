#include "createWalls.h"

float createTransformMatrices(Box aabb, float delta, ModelWrapper molecule)
{
	// Calculate AAAB dimensions

	Box bounds = molecule.getBounds();
	float width = abs(bounds.max.x - bounds.min.x);
	float height = abs(bounds.max.y - bounds.min.y);
	float depth = abs(bounds.max.z - bounds.min.z);
	vec3 center = vec3((aabb.min.x + aabb.max.x) / 2.0, (aabb.min.y + aabb.max.y) / 2.0, (aabb.min.z + aabb.max.z) / 2.0);

	mat4 matrices[5] = { mat4(1.0), mat4(1.0), mat4(1.0), mat4(1.0), mat4(1.0) };
	vec3 scaleFactor;
	vec3 translation;

	float hori = 20;
	float vert = 20;
	float dep = 20;


	// Left wall
	scaleFactor = vec3(delta / 2, height, depth);
	translation = vec3(center.x - (width * hori), center.y, center.z);
	matrices[0] = translate(scale(matrices[0], scaleFactor), translation);

	// Right wall
	translation.x = center.x + (width * hori);
	matrices[1] = translate(scale(matrices[1], scaleFactor), translation);

	// Top wall
	scaleFactor = vec3(width, delta / 2, depth);
	translation = vec3(center.x, center.y - (height * vert), center.z);
	matrices[2] = translate(scale(matrices[2], scaleFactor), translation);

	// Bottom wall
	translation.y = center.y + (height * vert);
	matrices[3] = translate(scale(matrices[3], scaleFactor), translation);

	// Back wall
	scaleFactor = vec3(width, height, delta / 2);
	translation = vec3(center.x, center.y, center.z - (depth * dep));
	matrices[4] = translate(scale(matrices[4], scaleFactor), translation);

	vec4 tempMin = matrices[4] * bounds.min;
	vec4 tempMax = matrices[4] * bounds.max;
	float backWallDiameter = glm::length(vec3(tempMin.x, tempMin.y, tempMin.z) - vec3(tempMax.x, tempMax.y, tempMax.z));
	printf("%f", backWallDiameter);
	
	GLuint wallsBuffer;
	glGenBuffers(1, &wallsBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, wallsBuffer);

	int binding = 20;
	glNamedBufferData(wallsBuffer, sizeof(matrices), &matrices, GL_STATIC_COPY_ARB);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, wallsBuffer);

	return backWallDiameter;
}