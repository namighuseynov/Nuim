#pragma once
#ifndef NUIM_SHADER_H
#define NUIM_SHADER_H


namespace Nuim {
	class Shader
	{
	public:
		Shader(const char* vertexShaderPath, const char* fragmentShaderPath);
		~Shader();
	public:
		void Use();
		unsigned int GetId();
		void SetInt(const char* attr, const int& value);
	public:

	private:
		int checkCompileOrLinkingError(const unsigned int& shaderOrProgram, const char* type);
		unsigned int id;
	};
}



#endif

