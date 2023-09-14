#include "nmpch.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Shader.h"

Nuim::Shader::Shader(std::string vertexShaderSource, std::string fragmentShaderSource)
{
	this->vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* source = vertexShaderSource.c_str();
	glShaderSource(GL_VERTEX_SHADER, 1, &source, NULL);
	glCompileShader(this->vertexShader);
	
	this->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	source = fragmentShaderSource.c_str();
	glShaderSource(GL_FRAGMENT_SHADER, 1, &source, NULL);
	glCompileShader(fragmentShader);

	this->ID = glCreateProgram();
	glAttachShader(this->ID, this->vertexShader);
	glAttachShader(this->ID, this->fragmentShader);
	glLinkProgram(this->ID);
}

Nuim::Shader::~Shader()
{
	glDeleteShader(this->vertexShader);
	glDeleteShader(this->fragmentShader);
	glDeleteProgram(this->ID);
}

void Nuim::Shader::Use()
{
	glUseProgram(this->ID);
}
