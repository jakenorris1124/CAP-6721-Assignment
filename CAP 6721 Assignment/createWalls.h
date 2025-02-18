#pragma once
// Include OpenGL Files
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "primitives.h"
#include <algorithm>
#include "model.h"
using namespace glm;

void createTransformMatrices(Box aabb, float delta, ModelWrapper molecule);