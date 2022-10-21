#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <streambuf>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Shader.h"



#define NUIM_SHADER 0
#define NUIM_PROGRAM 1

Nuim::Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath) {
	std::string vertexShaderSource = "";
	std::string fragmentShaderSource = "";
	std::ifstream vertexShaderFile;
	std::ifstream fragmentShaderFile;

	vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::failbit);
	try {
		vertexShaderFile.open(vertexShaderPath);
		fragmentShaderFile.open(fragmentShaderPath);
		std::stringstream vertexShaderStream, fragmentShaderStream;
		vertexShaderStream << vertexShaderFile.rdbuf();
		fragmentShaderStream << fragmentShaderFile.rdbuf();

		vertexShaderFile.close();
		fragmentShaderFile.close();

		vertexShaderSource = vertexShaderStream.str();
		fragmentShaderSource = fragmentShaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	char infoLog[512];
	int success;

	//convert sources to glchar
	const GLchar* vertexShaderSrc = vertexShaderSource.c_str();
	const GLchar* fragmentShaderSrc = fragmentShaderSource.c_str();
	unsigned int vertexShader, fragmentShader;
	//Creating shaders
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	//Set sources of shaders
	glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
	glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL);

	//Compile Shaders
	glCompileShader(vertexShader);
	this->checkCompileOrLinkingError(vertexShader, "SHADER");

	glCompileShader(fragmentShader);
	this->checkCompileOrLinkingError(fragmentShader, "SHADER");

	//Creating shader program
	this->id = glCreateProgram();
	glAttachShader(this->id, vertexShader);
	glAttachShader(this->id, fragmentShader);
	glLinkProgram(this->id);

	this->checkCompileOrLinkingError(this->id, "PROGRAM");
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Nuim::Shader::Use() {
	glUseProgram(this->id);
}

unsigned int Nuim::Shader::GetId() {
	return this->id;
}

int Nuim::Shader::checkCompileOrLinkingError(const unsigned int& shaderOrProgram, const char* type) {
	int success;
	char infoLog[512];
	if (type = "PROGRAM") {
		glGetProgramiv(shaderOrProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderOrProgram, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
			return -1;
		}
	}
	else if (type = "SHADER") {
		glGetShaderiv(shaderOrProgram, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shaderOrProgram, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << infoLog << std::endl;
			return -1;
		}
	}
	return 0;
}
