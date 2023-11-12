#pragma once 
#include "../Core/Core.h"

namespace Nuim {
    class Shader {
    public:
        Shader(std::string vertexShaderSrc, std::string fragmentShaderSrc);
        ~Shader();
    public:
        void Use();
    private:
        U32 id;
    };
}