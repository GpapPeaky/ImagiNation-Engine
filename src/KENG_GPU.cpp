#include "KENG_GPU.hpp"

namespace KENG::GPU {
    void RenderMapMask(OGL_Object* object, MapMaskMaterial& mmm) {
        OGL_Render(object, true);

        // Per-mode choices: which palette to sample, and whether realm highlighting applies
        GLuint activePal    = mmm.realmPalTex;
        int    selectedID   = mmm.selectedRealmID;

        switch (mmm.mm) {
            case OWNERSHIP:
                activePal  = mmm.realmPalTex;
                selectedID = mmm.selectedRealmID;
                break;

            case DEV:
                // Fall back to ownership if the dev palette hasn't been built yet
                activePal  = (mmm.devPalTex != 0) ? mmm.devPalTex : mmm.realmPalTex;
                // selectedID = -1;    // realm highlight is meaningless here, render everything
                break;
        }

        GLuint shader = object->mat.shader;

        if (mmm.provIdxTex != 0) {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, mmm.provIdxTex);
            glUniform1i(glGetUniformLocation(shader, "uProvinceIndexTex"), 1);
        }
        if (mmm.rlmIdxTex != 0) {
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, mmm.rlmIdxTex);
            glUniform1i(glGetUniformLocation(shader, "uRealmIndexTex"), 3);
        }
        if (activePal != 0) {
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, activePal);
            glUniform1i(glGetUniformLocation(shader, "uPaletteTex"), 2);
            glUniform1f(glGetUniformLocation(shader, "uProvinceCount"), mmm.provinceCount);
        }

        glUniform1i(glGetUniformLocation(shader, "uSelectedRealmID"), selectedID);

        OGL_Draw(object->mesh);

        glActiveTexture(GL_TEXTURE0);
    }

    GLuint BuildProvinceIndexTexture(const char* bitmapPath, KENG::ProvinceRegistry& preg, int& outW, int& outH){
        stbi_set_flip_vertically_on_load(true);

        int w, h, channels;
        unsigned char* data = stbi_load(bitmapPath, &w, &h, &channels, 0);
        if(!data){
            std::fprintf(stderr, "OGL_ERR: Failed to load province map %s (%s)\n", bitmapPath, stbi_failure_reason());
            outW = outH = 0;
            return 0;
        }

        std::vector<unsigned char> idBuffer(w * h * 2, 0); /* RG8: low byte, high byte */

        for(int i = 0; i < w * h; i++){
            unsigned char r = data[i * channels + 0];
            unsigned char g = data[i * channels + 1];
            unsigned char b = data[i * channels + 2];

            KENG::Province& prov = preg.GetProvince({r, g, b});

            llui id = 0; /* 0 = unowned / unmatched pixel (e.g. ocean) */
            if(prov.Id() != PROV::NULL_PROV_ID){
                id = prov.Id();
            }

            idBuffer[i * 2 + 0] = (unsigned char)(id & 0xFF);
            idBuffer[i * 2 + 1] = (unsigned char)((id >> 8) & 0xFF);
        }

        stbi_image_free(data);

        GLuint tex;
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); /* Critical: no mip/linear blending of IDs */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RG8, w, h, 0, GL_RG, GL_UNSIGNED_BYTE, idBuffer.data());
        glBindTexture(GL_TEXTURE_2D, 0);

        outW = w;
        outH = h;
        
        return tex;
    }

    GLuint BuildRealmIndexTexture(KENG::ProvinceRegistry& preg, KENG::RealmRegistry& rreg) {
        const int textureWidth = preg.Count() + 1;
    
        std::vector<unsigned char> idBuffer(textureWidth * 2, 0);
    
        for (int i = 0; i < preg.Count(); i++) {
            KENG::Province& prov = preg.GetProvince(i);
    
            const int provinceID = prov.Id();
    
            if (provinceID < 1 || provinceID >= textureWidth)
                continue;
    
            KENG::Realm& realm = rreg.GetOwnerOfProvince(provinceID);
    
            llui realmID = 0;
    
            if (realm.Id() != KENG::RLM::NULL_RLM_ID)
                realmID = realm.Id();
    
            idBuffer[provinceID * 2] =
                (unsigned char)(realmID & 0xFF);
    
            idBuffer[provinceID * 2 + 1] =
                (unsigned char)((realmID >> 8) & 0xFF);
        }
    
        GLuint tex;
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
    
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RG8,
            textureWidth,
            1,
            0,
            GL_RG,
            GL_UNSIGNED_BYTE,
            idBuffer.data()
        );
    
        glBindTexture(GL_TEXTURE_2D, 0);
    
        return tex;
    }

    void UpdateRealmIndexTexture(GLuint realmIndexTexture, int provinceID, int newRealmID) {
        unsigned char data[2] = {
            (unsigned char)(newRealmID & 0xFF),
            (unsigned char)((newRealmID >> 8) & 0xFF)
        };

        glBindTexture(GL_TEXTURE_2D, realmIndexTexture);

        glTexSubImage2D(
            GL_TEXTURE_2D,
            0,
            provinceID,
            0,
            1,
            1,
            GL_RG,
            GL_UNSIGNED_BYTE,
            data
        );

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    std::vector<std::array<float,3>> BuildRealmPaletteCPU(KENG::RealmRegistry& rreg, size_t provinceCount){
        /* Index 0 reserved for "no owner" pixels (ocean / unmatched) */
        std::vector<std::array<float,3>> palette(provinceCount + 1, {0.0f, 0.0f, 0.0f});

        for(auto& realm : rreg.Realms()){
            std::array<float,3> rgb = Utils::UnpackRGB_01(realm.Color());
            for(llui provId : realm.Provinces()){
                if(provId >= 1 && provId <= provinceCount){
                    palette[provId] = rgb;
                }
            }
        }

        return palette;
    }

    GLuint BuildRealmPaletteTexture(const std::vector<std::array<float,3>>& paletteCPU){
        GLuint tex;
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, (GLsizei)paletteCPU.size(), 1, 0, GL_RGB, GL_FLOAT, paletteCPU.data());
        glBindTexture(GL_TEXTURE_2D, 0);
        return tex;
    }

    /* Call this after any AddProvince/RemoveProvince changes ownership */
    void UpdateRealmPaletteEntry(GLuint paletteTex, llui provinceId, std::array<float, 3> rgb){
        glBindTexture(GL_TEXTURE_2D, paletteTex);
        glTexSubImage2D(GL_TEXTURE_2D, 0, (GLint)provinceId, 0, 1, 1, GL_RGB, GL_FLOAT, rgb.data());
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    std::vector<std::array<float,3>> BuildDevPaletteCPU(KENG::ProvinceRegistry& preg, size_t provinceCount) {
        std::vector<std::array<float,3>> pal(provinceCount + 1, {0.35f, 0.35f, 0.35f}); // grey for unknown/null ids

        int lo = INT_MAX, hi = INT_MIN;
        for (auto& p : preg.Provinces()) {
            if (p.Id() == KENG::PROV::NULL_PROV_ID) continue;
            int t = p.Dev().Total();
            lo = std::min(lo, t);
            hi = std::max(hi, t);
        }
        if (lo > hi) return pal;                                // no provinces
        float range = (hi > lo) ? float(hi - lo) : 1.f;         // all equal -> avoid divide by zero

        for (auto& p : preg.Provinces()) {
            if (p.Id() == KENG::PROV::NULL_PROV_ID || p.Id() > provinceCount) continue;
            
            float t = (p.Dev().Total() - lo) / range;         // 0 = lowest, 1 = highest
            
            auto c = Utils::HSV2RGB(t * 120.0f, 0.85f, 0.9f);
            pal[p.Id()] = { c[0] / 255.0f, c[1] / 255.0f, c[2] / 255.0f };
        }

        return pal;
    }

    GLuint BuildDevPaletteTexture(KENG::ProvinceRegistry& preg, size_t provinceCount) {
        return BuildRealmPaletteTexture(BuildDevPaletteCPU(preg, provinceCount));
    }

    void UpdateDevPaletteTexture(GLuint& devPalTex, KENG::ProvinceRegistry& preg, size_t provinceCount) {
        if (devPalTex) glDeleteTextures(1, &devPalTex);
        devPalTex = BuildDevPaletteTexture(preg, provinceCount);
    }

    /* ----------------------------------------------------------------------------------------------------------- */

    std::vector<glm::vec3> BuildProvinceOutlineVertices(const char* bitmapPath, int& outW, int& outH){
        std::vector<glm::vec3> vertices;

        stbi_set_flip_vertically_on_load(true);

        int w, h, channels;
        unsigned char* data = stbi_load(bitmapPath, &w, &h, &channels, 0);

        if(!data){
            std::cerr << ("[Error] Failed loading province map\n");
            outW = outH = 0;
            return vertices;
        }

        auto getColorID = [&](int x, int y) {
            int index = (y * w + x) * channels;
            unsigned char r = data[index + 0];
            unsigned char g = data[index + 1];
            unsigned char b = data[index + 2];
            return (r << 16) | (g << 8) | b;
        };

        auto normX = [&](int x){ return ((float)x / (float)w) * 2.0f - 1.0f; };
        auto normY = [&](int y){ return ((float)y / (float)h) * 2.0f - 1.0f; };

        for(int y = 0; y < h; y++){
            for(int x = 0; x < w; x++){

                int id = getColorID(x, y);

                // Vertical edge: boundary is at x+1, spans y..y+1
                if(x < w - 1){
                    int right = getColorID(x + 1, y);
                    if(id != right){
                        float px = normX(x + 1);

                        vertices.push_back({ px, normY(y),     0.0f });
                        vertices.push_back({ px, normY(y + 1), 0.0f });
                    }
                }

                // Horizontal edge: boundary is at y+1, spans x..x+1
                if(y < h - 1){
                    int down = getColorID(x, y + 1);
                    if(id != down){
                        float py = normY(y + 1);

                        vertices.push_back({ normX(x),     py, 0.0f });
                        vertices.push_back({ normX(x + 1), py, 0.0f });
                    }
                }
            }
        }

        stbi_image_free(data);

        outW = w;
        outH = h;

        std::cout << "[Info] Generated " << vertices.size() << " outline vertices\n";

        return vertices;
    }
    void RenderProvinceOutlines(OGL_Object* object, glm::vec3 color) {
        OGL_Render(object, true);

        glUniform3f(
            glGetUniformLocation(
                object->mat.shader,
                "uColor"
            ),
            color.x,
            color.y,
            color.z
        );


        glBindVertexArray(
            object->mesh->VAO
        );


        glDrawArrays(
            GL_LINES,
            0,
            object->mesh->verticesCount
        );


        glBindVertexArray(0);
    }
}