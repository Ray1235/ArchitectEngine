#include "../common.h"
#include "r_display.h"
#include <GL\glew.h>
#include <SFML\OpenGL.hpp>
#include <gl\GL.h>

float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f,  0.5f, 0.0f
};

unsigned int VBO;
unsigned int VAO;

unsigned int shaderProgram;

unsigned int vertexShader;
unsigned int fragmentShader;

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

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
	glViewport(0, 0, 640, 480);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	shaderProgram = glCreateProgram();

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		Com_Error(ERR_FATAL, "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s", infoLog);
	}

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		Com_Error(ERR_FATAL, "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s", infoLog);
	}

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		Com_Error(ERR_FATAL, "Error linking shader program: %s", infoLog);
	}

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 1. then set the vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	return true;
}

void R_Shutdown()
{
	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	R_ShutdownDisplay();
}

void R_BeginFrame()
{
	glClear(GL_COLOR_BUFFER_BIT);	
}

void R_Frame()
{
	
	// 2. use our shader program when we want to render an object
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	
}

void R_EndFrame()
{
	R_GetDisplay()->display();
}