#include "Shader.hpp"
#include "../Platform/glad/glad.h"
#include <GLFW/glfw3.h>

namespace Nuim {
    Shader::Shader(std::string vertexShaderPath, std::string fragmentShaderPath) {
        std::string vertexShaderSrc, fragmentShaderSrc;
        std::ifstream vertexShaderFile, fragmentShaderFile;
        vertexShaderFile.exceptions (std::ifstream::badbit | std::ifstream::failbit);
        fragmentShaderFile.exceptions (std::ifstream::badbit | std::ifstream::failbit);

        try {
            vertexShaderFile.open(vertexShaderPath);
            fragmentShaderFile.open(fragmentShaderPath);

            std::stringstream vertexShaderStream, fragmentShaderStream;
            vertexShaderStream << vertexShaderFile.rdbuf();
            fragmentShaderStream << fragmentShaderFile.rdbuf();

            vertexShaderFile.close();
            fragmentShaderFile.close();

            vertexShaderSrc = vertexShaderStream.str();
            fragmentShaderSrc = fragmentShaderStream.str();
        }
        catch (std::ifstream::failure e) {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        const char* vertexShaderCode = vertexShaderSrc.c_str();
        const char* fragmentShaderCode = fragmentShaderSrc.c_str();
        U32 vertexShader, fragmentShader; 
        I32 success;
        char infoLog[512];

        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1,  &vertexShaderCode, NULL);
        glCompileShader(vertexShader);
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        };
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        };

        this->id = glCreateProgram();
        glAttachShader(this->id, vertexShader);
        glAttachShader(this->id, fragmentShader);
        glLinkProgram(this->id);
        glGetProgramiv(this->id, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(this->id, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    Shader::~Shader() {
        glDeleteProgram(this->id);
    }

    void Shader::Use() {
        glUseProgram(this->id);
    }
}