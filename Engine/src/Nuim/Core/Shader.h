#pragma once

namespace Nuim {
	static const char* LoadShader(std::string path) {
		std::string line;
		std::string allLine;
		std::ifstream shaderFile;
		shaderFile.open(path, std::ios::in);
		if (shaderFile.is_open()) {
			while (std::getline(shaderFile, line)) {
				allLine+=line;
			}
			shaderFile.close();
			return allLine.c_str();
		}
		else {
			std::cerr << "Error opening the file" << std::endl;
			return nullptr;
		}
	}

	class Shader {
	public:
		Shader(std::string vertexShaderSource, std::string fragmentShaderSource);
		~Shader();
	public:
		void Use();
	private:
		unsigned int fragmentShader;
		unsigned int vertexShader;
		unsigned int ID;
	};
}

