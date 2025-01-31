#pragma once
// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>

// Include OpenGL Files
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
using namespace glm;

// Vars
extern GLFWwindow* window;
extern const GLchar* computeShaderPath;
extern const GLchar* vertShaderPath;
extern const GLchar* fragShaderPath;
extern GLuint vao;
extern GLuint texture_out;
extern GLuint paletteBuffer;
extern const int WindowWidth;
extern const int WindowHeight;
extern GLuint drawProgram;
extern GLuint computeProgram;
extern int workgroups[3];

void init();
GLuint setVertices();
void checkShaderProgram(GLuint program);
void checkShaderCompilation(GLuint shader, const char* shaderType);
GLuint loadShader(const GLchar* shaderFilePath, const char* shaderType);
GLuint setImageStore();
GLint* computeWorkGroupSize();
void setupDrawProgram();
void setupComputeProgram();

void debug();
void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam);


