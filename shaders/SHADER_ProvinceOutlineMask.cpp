#include "SHADER_ProvinceOutlineMask.hpp"

std::string INATE_SHADER_ProvOutlineMaskF = R"(
#version 330 core

uniform vec3 uColor;

out vec4 FragColor;

void main()
{
    FragColor = vec4(uColor,1.0);
}
)";

std::string INATE_SHADER_ProvOutlineMaskV = R"(
)";