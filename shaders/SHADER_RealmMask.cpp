#include "SHADER_RealmMask.hpp"

std::string INATE_SHADER_RealmMaskF = R"(
#version 330 core

uniform sampler2D uIndexTex;
// the baked ID map
//
// Same size as province_map.png
// each pixel looks up RGB in colorToID -> gets prov id
// 1500 entries fit in a 16 bit integer
// use GL_NEAREST for it to avoid bad colours
//

uniform sampler2D uPaletteTex;
// 1500x1 realm-color lookup
//
// Use GL_NEAREST here as well
// this basically matches an id to a pixel of the texture which contains
// the owner colour. When ownership changes we simply change the pixel colours
// O(1)

uniform float uProvinceCount;

in vec2 TexCoord;
out vec4 FragColor;

void main() {
    vec2 idColor = texture(uIndexTex, 1.0 - TexCoord).rg;
    float id = floor(idColor.r * 255.0 + 0.5) + floor(idColor.g * 255.0 + 0.5) * 256.0;

    float u = (id + 0.5) / uProvinceCount;
    vec3 realmColor = texture(uPaletteTex, vec2(u, 0.5)).rgb;

    FragColor = vec4(realmColor, 1.0);
}
)";

std::string INATE_SHADER_RealmMaskV = R"(
)";