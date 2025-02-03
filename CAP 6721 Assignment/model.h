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
		Box bounds;
		GLuint storageBuffer;

	public:
		ModelWrapper(std::string modelName) : name(modelName) 
		{
			glGenBuffers(1, &storageBuffer);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, storageBuffer);
		}

		std::string getName() { return name; }
		GLuint getStorageBuffer() { return storageBuffer; }

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

			struct Model
			{
				std::vector<Sphere> spheres;
				Box bounds;
			};
			Model payload{ spheres = spheres, bounds = bounds };

			glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Model), &payload, GL_STATIC_DRAW);
		}

		void bind()
		{
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, storageBuffer);
		}
};