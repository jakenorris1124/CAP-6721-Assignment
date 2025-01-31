// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Include Helper Files
#include "setupShaderPrograms.h"
#include "createQuadVAO.h"
#include "debug.h"


vec3 eye, at, up;
float fov;

uint quadVAO;

void setUniforms()
{
	eye = vec3(0.0, 0.0, 0.0);
	at = vec3(0.0, 0.0, -1.0);
	up = vec3(0.0, 1.0, 0.0);

	fov = 90.0;
}


void compute()
{
	glUseProgram(computeProgram);
	glBindImageTexture(0, texture_out, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, paletteBuffer);

	// Uniforms
	glUniform3f(glGetUniformLocation(computeProgram, "eye"), eye.x, eye.y, eye.z);
	glUniform3f(glGetUniformLocation(computeProgram, "at"), at.x, at.y, at.z);
	glUniform3f(glGetUniformLocation(computeProgram, "up"), up.x, up.y, up.z);
	glUniform2f(glGetUniformLocation(computeProgram, "resolution"), WindowWidth, WindowHeight);
	glUniform1f(glGetUniformLocation(computeProgram, "fov"), fov);

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
	setUniforms();
	setupDrawProgram();
	setupComputeProgram();
	quadVAO = createQuadVAO();
	do {
		glClear(GL_COLOR_BUFFER_BIT);
		compute();
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
		draw();
		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);
}