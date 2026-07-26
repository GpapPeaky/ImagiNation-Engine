#pragma once

#include "KENG_ProvinceRegistry.hpp"
#include "KENG_RealmRegistry.hpp"

#include "../../Basic-OpenGL/ThirdParty/stb_image/stb_image.h"

namespace KENG::GPU {
    typedef struct RealmMaskMaterial {
        OGL_Material mat;
        GLuint idxTex = 0;          /* Province-ID map, baked once from province_map.png */
        GLuint palTex = 0;          /* provinceCount+1 wide, texel[id] = owning realm color */
        float provinceCount = 0.0f;
    } RealmMaskMaterial;

    // Custom render function that passes idxTex, palTex and province count as uniforms
    void RenderRealmMask(OGL_Object* object, RealmMaskMaterial& rmm);

    // Build the index texture, called once
    GLuint BuildProvinceIndexTexture(const char* bitmapPath, KENG::ProvinceRegistry& preg, int& outW, int& outH);

    // Create the palette texture data in CPU-side
    std::vector<std::array<float,3>> BuildPaletteCPU(KENG::RealmRegistry& rreg, size_t provinceCount);

    // Palette texture data GPU-side
    GLuint CreatePaletteTexture(const std::vector<std::array<float,3>>& paletteCPU);

    // Palette texture data update, GPU-side, make sure it is called after a change in ownership
    // meaning the provCount * 1 px texture needs to update one of its' pixels
    void UpdatePaletteEntry(GLuint paletteTex, llui provinceId, std::array<float, 3> rgb);

    /* ----------------------------------------------------------------------------------------------------------- */

    std::vector<glm::vec3> BuildProvinceOutlineVertices(const char* bitmapPath, int& outW, int& out);

    void RenderProvinceOutlines(OGL_Object* object, glm::vec3 color);
}