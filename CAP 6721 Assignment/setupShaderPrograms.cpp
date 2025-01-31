#include "setupShaderPrograms.h"

// Vars
GLFWwindow* window;
const GLchar* computeShaderPath = "./compute.comp",
* vertShaderPath = "./compute.vert",
* fragShaderPath = "./compute.frag";
GLuint vao;
GLuint texture_out;
GLuint paletteBuffer;
const int WindowWidth = 640;
const int WindowHeight = 480;
GLuint drawProgram;
GLuint computeProgram;
int workgroups[3];

void init()
{
	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(WindowWidth, WindowHeight, "CAP 6721 Assignment", NULL, NULL);
	glfwMakeContextCurrent(window);

	glewInit();
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
}

GLuint setVertices() {
	// set vertices to vao and vbo for rendering quad
	float vertices[] = {
		// 5 float: 3 for viewport position followed by 2 for texture coords 
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom left
		-1.0f, 1.0f,  0.0f, 0.0f, 1.0f, // bottom right
		1.0f,  -1.0f, 0.0f, 1.0f, 0.0f, // top left
		1.0f,  1.0f,  0.0f, 1.0f, 1.0f  // top right
	};
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
		(void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glDeleteBuffers(1, &vbo);
	glBindVertexArray(0);
	return vao;
}

void checkShaderProgram(GLuint program) {
	int success;
	char infoLog[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (success == 0) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER PROGRAM LINK_FAILED\n"
			<< infoLog << std::endl;
	}
}
void checkShaderCompilation(GLuint shader, const char* shaderType) {
	// check the shader compilation
	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (success == 0) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::" << shaderType << "::COMPILATION_FAILED\n"
			<< infoLog << std::endl;
	}
}
GLuint loadShader(const GLchar* shaderFilePath,
	const char* shaderType) {
	// load shader file from system
	GLuint shader;
	std::string stype(shaderType);
	if (stype == "FRAGMENT") {
		shader = glCreateShader(GL_FRAGMENT_SHADER);
	}
	else if (stype == "VERTEX") {
		shader = glCreateShader(GL_VERTEX_SHADER);
	}
	else if (stype == "COMPUTE") {
		shader = glCreateShader(GL_COMPUTE_SHADER);
	}
	else {
		std::cout << "Unknown shader type:\n" << shaderType << std::endl;
	}
	std::ifstream shdrFileStream;
	shdrFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	std::string shaderCodeStr;
	try {
		shdrFileStream.open(shaderFilePath);
		std::stringstream shaderSStream;
		shaderSStream << shdrFileStream.rdbuf();
		shdrFileStream.close();
		shaderCodeStr = shaderSStream.str();
	}
	catch (std::ifstream::failure e) {
		//
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* shaderCode = shaderCodeStr.c_str();

	// lets source the shader
	glShaderSource(shader, 1, &shaderCode, NULL);
	glCompileShader(shader);

	// a sanity check for unsuccessful compilations
	checkShaderCompilation(shader, shaderType);
	return shader;
}

GLuint setImageStore()
{
	GLuint texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, WindowWidth, WindowHeight, 0,
		GL_RGBA, GL_FLOAT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0); // unbind

	return texture;
}

GLint* computeWorkGroupSize()
{
	GLint workGroupSize[3];

	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &workGroupSize[0]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &workGroupSize[1]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &workGroupSize[2]);

	
	return workGroupSize;
}

void setupDrawProgram()
{
	fprintf(stdout, "Setting up the draw Program\n");
	vao = setVertices();
	texture_out = setImageStore();
	GLuint vertShader = loadShader(vertShaderPath, "VERTEX");
	GLuint fragShader = loadShader(fragShaderPath, "FRAGMENT");
	drawProgram = glCreateProgram();
	glAttachShader(drawProgram, vertShader);
	glAttachShader(drawProgram, fragShader);
	glLinkProgram(drawProgram);
	checkShaderProgram(drawProgram);
	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
	fprintf(stdout, "Draw program set up complete\n");
}
void setupComputeProgram()
{
	fprintf(stdout, "Setting up the Compute Program\n");
	computeProgram = glCreateProgram();
	GLuint cshader = loadShader(computeShaderPath, "COMPUTE");
	glAttachShader(computeProgram, cshader);
	glLinkProgram(computeProgram);
	checkShaderProgram(computeProgram);
	glDeleteShader(cshader);
	fprintf(stdout, "Compute program set up complete\n");
	GLint* workGroupSize = computeWorkGroupSize();
	glGetProgramiv(computeProgram, GL_COMPUTE_WORK_GROUP_SIZE, workGroupSize);
	//fprintf(stdout, "Local Work Group size: %d, %d, %d\n", workGroupSize[0], workGroupSize[1], workGroupSize[2]);

	workgroups[0] = (WindowWidth + workGroupSize[0] - 1) / workGroupSize[0];
	workgroups[1] = (WindowHeight + workGroupSize[1] - 1) / workGroupSize[1];
	workgroups[2] = 1;
	fprintf(stdout, "Compute Work GroupS : % d, % d, % d\n", workgroups[0], workgroups[1], workgroups[2]);

	glGenBuffers(1, &paletteBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, paletteBuffer);
}
