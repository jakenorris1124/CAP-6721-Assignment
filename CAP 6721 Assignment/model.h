#pragma once
// Include standard headers
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>

// Include OpenGL Files
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
using namespace glm;

#include "primitives.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class ModelWrapper
{
	private:
		std::string name;
		std::vector<Sphere> spheres;
		GLuint sphereBuffer;
		Box bounds;
		GLuint boundsBuffer;

		int binding = 0;
		void bindData(void* data, int size, GLuint buffer)
		{
			glNamedBufferData(buffer, size, data, GL_STATIC_DRAW);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, buffer);

			binding++;
		}

	public:
		ModelWrapper(std::string modelName) : name(modelName) 
		{
			// Create sphere buffer
			glGenBuffers(1, &sphereBuffer);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, sphereBuffer);

			// Create bounds buffer
			glGenBuffers(1, &boundsBuffer);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, boundsBuffer);
		}

		std::string getName() { return name; }

		void load(std::string path) 
		{
			std::ifstream file(path);
			json data = json::parse(file);

			int numSpheres = data[name]["quadruples"].size() / 4;
			for (auto i = 0; i < numSpheres; i++)
			{
				Sphere sphere;
				sphere.center = { data[name]["quadruples"][i], data[name]["quadruples"][i + 1], data[name]["quadruples"][i + 2] };
				sphere.radius = data[name]["quadruples"][i + 3];
				sphere.color = { data[name]["color"][i], data[name]["color"][i + 1], data[name]["color"][i + 2] };

				spheres.push_back(sphere);
			}

			bounds.min = { data[name]["box"]["min"][0], data[name]["box"]["min"][1], data[name]["box"]["min"][2] };
			bounds.max = { data[name]["box"]["max"][0], data[name]["box"]["max"][1], data[name]["box"]["max"][2] };

			bindData(&bounds, sizeof(bounds), boundsBuffer);
			bindData(&spheres, sizeof(Sphere) * spheres.size(), sphereBuffer);
		}
};