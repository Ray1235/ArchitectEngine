#include "../common.h"
#include "r_display.h"
#include <GL\glew.h>
#include <SFML\OpenGL.hpp>
#include <gl\GL.h>

float vertices[] = {
	-16.5f, -0.5f, 0.0f,
	16.5f, -16.5f, 0.0f,
	16.0f,  16.5f, 0.0f
};

unsigned int VBO;
unsigned int VAO;

unsigned int shaderProgram;

unsigned int vertexShader;
const char *vertexShaderSource = "#version 330 core \n \
layout(location = 0) in vec3 aPos; \n\
 \
void main()\n \
{ \
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); \
}";

unsigned int fragmentShader;
const char *fragmentShaderSource = "#version 330 core\n \
out vec4 FragColor;\n \
 \
void main()\n \
{ \
	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); \
} ";

bool R_Init()
{
	if(!R_InitDisplay(640, 480))
	{
		return false;
	}

	if (!R_MaterialSystem_Init())
	{
		return false;
	}
	glewInit();
	/*
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	shaderProgram = glCreateProgram();

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 1. then set the vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	*/

	return true;
}

void R_Shutdown()
{
	/*
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	*/
	R_ShutdownDisplay();
}

void R_BeginFrame()
{
	glClear(GL_COLOR_BUFFER_BIT);	
	//al_clear_to_color(al_map_rgba(0, 0, 0, 255));
}

void R_Frame()
{
	/*
	// 2. use our shader program when we want to render an object
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	*/
}

void R_EndFrame()
{
	R_GetDisplay()->display();
	//al_flip_display();
}