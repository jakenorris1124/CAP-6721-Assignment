#pragma once
// Include OpenGL Files
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
using namespace glm;

struct Sphere
{
	glm::vec3 center;
	glm::vec3 color;
	float radius;
};

struct Box
{
	glm::vec3 min;
	glm::vec3 max;
};