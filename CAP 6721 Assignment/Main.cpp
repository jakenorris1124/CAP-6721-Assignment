// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Include Helper Files
#include "setupShaderPrograms.h"
#include "createQuadVAO.h"
#include "debug.h"
#include "model.h"
#include "camera.h"

uint quadVAO;

const std::string modelPath = "./molecules.json";
const std::string modelName = "Ethanol";

void compute(Camera* camera)
{
	glUseProgram(computeProgram);
	glBindImageTexture(0, texture_out, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, paletteBuffer);

	// Uniforms
	(*camera).update();
	glUniform2f(glGetUniformLocation(computeProgram, "resolution"), WindowWidth, WindowHeight);

	glDispatchCompute(workgroups[0], workgroups[1], 1);
}

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(drawProgram);
	glBindVertexArray(quadVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_out);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
}

int main(int argc, char* argv[]) 
{
	init();
	debug();
	setupDrawProgram();
	setupComputeProgram();
	quadVAO = createQuadVAO();

	ModelWrapper molecule = ModelWrapper(modelName);
	molecule.load(modelPath);
	vec3 center = molecule.getCenter();
	
	Camera camera = Camera(
		vec3(center.x, center.y, center.z + molecule.getDiagonal()),
		center,
		vec3(0.0, 1.0, 0.0),
		90.0
	);
	camera.activate(computeProgram);
	camera.setRotation(1.0, vec3(0.0, center.y, 0.0));

	do {
		glClear(GL_COLOR_BUFFER_BIT);
		compute(&camera);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
		draw();
		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);
}