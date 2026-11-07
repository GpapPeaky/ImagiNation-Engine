#pragma once

#include "KENG_ProvinceRegistry.hpp"
#include "KENG_RealmRegistry.hpp"

#include <climits>
#include <cmath>

#include "../../Basic-OpenGL/ThirdParty/stb_image/stb_image.h"

namespace KENG::GPU {
    typedef enum MapMode {
        OWNERSHIP,
        DEV
    } MapMode;

    typedef struct MapMaskMaterial {
        OGL_Material mat;
        GLuint provIdxTex = 0;          /* Province-ID map, baked once from province_map.png */
        GLuint rlmIdxTex  = 0;          
        /*
            Realm-ID map, baked after each change of ownership.
            
            provinceCount+1 wide, 1d texture for faster lookup

            Every entry contains the province's owning realm ID.
        */
        GLuint realmPalTex = 0;              /* provinceCount+1 wide, texel[id] = owning realm color */
        GLuint devPalTex   = 0;              /* Development palette texture, update per dev change */
        float provinceCount = 0.0f;
        int selectedRealmID = -1;       /* Selected realm color for highlighting */

        MapMode mm = OWNERSHIP;
    } MapMaskMaterial;

    // Custom render function that passes idxTex, realmPalTex and province count as uniforms
    void RenderMapMask(OGL_Object* object, MapMaskMaterial& mmm);

    // Build the index texture, called once
    GLuint BuildProvinceIndexTexture(const char* bitmapPath, KENG::ProvinceRegistry& preg, int& outW, int& outH);

    // Build the realm texture, called once
    GLuint BuildRealmIndexTexture(KENG::ProvinceRegistry& preg, KENG::RealmRegistry& rreg);

    // Update of the realm texture, after ownership change, no need to be built again
    void UpdateRealmIndexTexture(GLuint realmIndexTexture, int provinceID, int newRealmID);

    // Create the palette texture data in CPU-side
    std::vector<std::array<float,3>> BuildRealmPaletteCPU(KENG::RealmRegistry& rreg, size_t provinceCount);

    // Palette texture data GPU-side
    GLuint BuildRealmPaletteTexture(const std::vector<std::array<float,3>>& paletteCPU);

    // Palette texture data update, GPU-side, make sure it is called after a change in ownership
    // meaning the provCount * 1 px texture needs to update one of its' pixels
    void UpdateRealmPaletteEntry(GLuint paletteTex, llui provinceId, std::array<float, 3> rgb);

    // CPU side, so it can be tested or reused
    std::vector<std::array<float,3>> BuildDevPaletteCPU(KENG::ProvinceRegistry& preg, size_t provinceCount);

    // GPU side, thin wrapper over BuildRealmPaletteTexture
    GLuint BuildDevPaletteTexture(KENG::ProvinceRegistry& preg, size_t provinceCount);

    // Min/max can change when any province changes, so this recomputes and re-uploads the whole (tiny) texture
    void UpdateDevPaletteTexture(GLuint& devPalTex, KENG::ProvinceRegistry& preg, size_t provinceCount);

    /* ----------------------------------------------------------------------------------------------------------- */

    std::vector<glm::vec3> BuildProvinceOutlineVertices(const char* bitmapPath, int& outW, int& out);

    void RenderProvinceOutlines(OGL_Object* object, glm::vec3 color);
}