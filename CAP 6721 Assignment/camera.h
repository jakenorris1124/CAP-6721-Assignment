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
#include <glm/ext/matrix_transform.hpp>
using namespace glm;

class Camera
{
	private:
		vec3 eye;
		vec3 at;
		vec3 up;
		float fov;

		float degrees = 0.0;
		mat4 rotationMatrix;
		vec3 axis;

		GLuint computeProgram = NAN;

	public:
		Camera(vec3 eye_, vec3 at_, vec3 up_, float fov_) : eye(eye_), at(at_), up(up_), fov(fov_) {}

		void activate(GLuint compute)
		{
			computeProgram = compute;
		}

		void update()
		{
			if (computeProgram == NAN)
				return;

			if (degrees != 0.0)
				eye = vec3(rotationMatrix * vec4(eye, 1.0));

			glUniform3f(glGetUniformLocation(computeProgram, "eye"), eye.x, eye.y, eye.z);
			glUniform3f(glGetUniformLocation(computeProgram, "at"), at.x, at.y, at.z);
			glUniform3f(glGetUniformLocation(computeProgram, "up"), up.x, up.y, up.z);
			glUniform1f(glGetUniformLocation(computeProgram, "fov"), fov);
		}

		void setEye(vec3 eye_) { eye = eye_; }
		vec3 getEye() { return eye; }

		void setAt(vec3 at_) { at = at_; }
		vec3 getAt() { return at; }

		void setUp(vec3 up_) { up = up_; }
		vec3 getUp() { return up; }

		void setFov(float fov_) { fov = fov_; }
		float getFov() { return fov; }
		
		void setRotation(float degrees_, vec3 axis_ = vec3(0, 0, 0)) 
		{ 
			degrees = degrees_; 
			axis = axis_;

			rotationMatrix = mat4(1.0f);
			rotationMatrix = glm::rotate(rotationMatrix, glm::radians(degrees), axis);
		}
		float getRotation() { return degrees; }
};