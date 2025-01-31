#include "createQuadVAO.h"

unsigned int createQuadVAO()
{
    // configure VAO/VBO
    unsigned int VBO;
    float vertices[] = { 
        // First triangle
        -1.0, -1.0,
        1.0, -1.0f,
		1.0, 1.0,
        // Second triangle
		-1.0, 1.0,
        -1.0, -1.0,
        1.0, 1.0
    };
	unsigned int quadVAO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &VBO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);  
    glBindVertexArray(0);
	return quadVAO;
}