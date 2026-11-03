#pragma once

#include "KENG_ProvinceRegistry.hpp"
#include "KENG_RealmRegistry.hpp"

#include "../../Basic-OpenGL/ThirdParty/stb_image/stb_image.h"

namespace KENG::GPU {
    typedef struct RealmMaskMaterial {
        OGL_Material mat;
        GLuint provIdxTex = 0;          /* Province-ID map, baked once from province_map.png */
        GLuint rlmIdxTex  = 0;          
        /*
            Realm-ID map, baked after each change of ownership.
            
            provinceCount+1 wide, 1d texture for faster lookup

            Every entry contains the province's owning realm ID.
        */
        GLuint palTex = 0;              /* provinceCount+1 wide, texel[id] = owning realm color */
        float provinceCount = 0.0f;
        int selectedRealmID = -1;       /* Selected realm color for highlighting */
    } RealmMaskMaterial;

    // Custom render function that passes idxTex, palTex and province count as uniforms
    void RenderRealmMask(OGL_Object* object, RealmMaskMaterial& rmm);

    // Build the index texture, called once
    GLuint BuildProvinceIndexTexture(const char* bitmapPath, KENG::ProvinceRegistry& preg, int& outW, int& outH);

    // Build the realm texture, called multiple times, once at the start, and after each ownership change
    GLuint BuildRealmIndexTexture(KENG::ProvinceRegistry& preg, KENG::RealmRegistry& rreg);

    // Update of the realm texture, after ownership change, no need to be built again
    void UpdateRealmIndexTexture(GLuint realmIndexTexture, int provinceID, int newRealmID);

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