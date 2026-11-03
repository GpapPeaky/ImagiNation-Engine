#include "SHADER_RealmMask.hpp"

std::string INATE_SHADER_RealmMaskF = R"(
#version 330 core

uniform sampler2D uProvinceIndexTex;
// Baked province ID map.
//
// Same dimensions as province_map.png.
// Each pixel stores a 16-bit province ID encoded in RG.
// The province ID is extracted from the original RGB province map
// using colorToID during texture generation.
//
// GL_NEAREST is required to prevent interpolation between IDs.

uniform sampler2D uRealmIndexTex;
// Dynamic province-to-realm lookup table.
//
// Dimensions: (provinceCount + 1) x 1.
// Each texel, indexed by province ID, stores its current owning realm ID
// encoded as a 16-bit integer in RG.
//
// Updated when province ownership changes, without modifying
// the baked province index texture.
//
// GL_NEAREST is required to prevent interpolation between IDs.

uniform sampler2D uPaletteTex;
// Dynamic province-to-realm-color lookup table.
//
// Dimensions: (provinceCount + 1) x 1.
// Each texel, indexed by province ID, stores the RGB color
// of the realm that currently owns that province.
//
// Updated when ownership or realm colors change.
//
// GL_NEAREST is required to preserve exact palette entries.

uniform float uProvinceCount;
uniform int  uSelectedRealmID;

in vec2 TexCoord;
out vec4 FragColor;

void main() {
    vec2 provIdColor = texture(uProvinceIndexTex, 1.0 - TexCoord).rg;
    float provId = int(floor(provIdColor.r * 255.0 + 0.5)) | (int(floor(provIdColor.g * 255.0 + 0.5)) << 8);
    
    float u = (provId + 0.5) / uProvinceCount;

    vec2 rlmIdColor = texture(uRealmIndexTex, vec2(u, 0.5)).rg;
    int rlmId = int(floor(rlmIdColor.r * 255.0 + 0.5)) | (int(floor(rlmIdColor.g * 255.0 + 0.5)) << 8);

    vec3 realmColor = texture(uPaletteTex, vec2(u, 0.5)).rgb;

    if (uSelectedRealmID == -1)
    {
        // Nothing selected: render everything normally.
        FragColor = vec4(realmColor, 1.0);
    }
    else if (rlmId == uSelectedRealmID)
    {
        // Selected realm.
        FragColor = vec4(realmColor, 1.0);
    }
    else
    {
        // Other realms.
        FragColor = vec4(realmColor, 0.35);
    }
}
)";

std::string INATE_SHADER_RealmMaskV = R"(
)";