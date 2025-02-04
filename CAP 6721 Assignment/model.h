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

		int binding = 1;
		void bindData(void* data, int size, GLuint buffer)
		{
			glNamedBufferData(buffer, size, data, GL_STATIC_COPY_ARB);
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
		vec3 getCenter() { return bounds.min + ((bounds.max - bounds.min) * 0.5f); }
		float getDiagonal() { return glm::length(bounds.max - bounds.min); }

		void load(std::string path) 
		{
			std::ifstream file(path);
			json data = json::parse(file);

			int numSpheres = data[name]["quadruples"].size() / 4;
			for (auto i = 0; i < numSpheres; i++)
			{
				int quad = i * 4;
				int tri = i * 3;

				Sphere sphere;
				sphere.center = vec3(data[name]["quadruples"][quad], data[name]["quadruples"][quad + 1], data[name]["quadruples"][quad + 2]);
				sphere.radius = data[name]["quadruples"][quad + 3];
				sphere.color = vec3(data[name]["color"][tri], data[name]["color"][tri + 1], data[name]["color"][tri + 2]);
				sphere.color /= 255.0;

				spheres.push_back(sphere);
			}

			bounds.min = vec3(data[name]["box"]["min"][0], data[name]["box"]["min"][1], data[name]["box"]["min"][2]);
			bounds.max = vec3(data[name]["box"]["max"][0], data[name]["box"]["max"][1], data[name]["box"]["max"][2]);

			bindData(&bounds, sizeof(Box), boundsBuffer);
			bindData(&spheres.data()[0], sizeof(Sphere) * spheres.size(), sphereBuffer);

		}
};