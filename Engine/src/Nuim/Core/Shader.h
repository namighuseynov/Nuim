#pragma once

namespace Nuim {
	static const char* LoadShader(std::string path) {
		std::ifstream shaderFile;
		std::stringstream shaderStream;
		shaderFile.open(path, std::ios::in);
		if (shaderFile.is_open()) {
			shaderStream << shaderFile.rdbuf();
			shaderFile.close();
			return shaderStream.str().c_str();
		}
		else {
			std::cerr << "Error opening the file" << std::endl;
			return nullptr;
		}
	}

	class Shader {
	public:
		Shader(const char* vertexShaderSource, const char* fragmentShaderSource);
		~Shader();
	public:
		void Use();
		void SetColor(float r, float g, float b);
	private:
		unsigned int ID;
	};
}

