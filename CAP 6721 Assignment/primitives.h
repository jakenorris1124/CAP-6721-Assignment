#pragma once
// Include OpenGL Files
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
using namespace glm;

struct Sphere
{
	glm::vec4 color;
	glm::vec3 center;
	float radius;
};

struct Box
{
	glm::vec4 min;
	glm::vec4 max;
};