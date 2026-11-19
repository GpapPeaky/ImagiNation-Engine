#pragma once

#include "KENG_Utils.hpp"
#include "KENG_ProvinceRegistry.hpp"

#include <algorithm>
#include <unordered_map>
#include <vector>
#include <limits>
#include <climits>
#include <cmath>

#include "../../Basic-OpenGL/ThirdParty/stb_image/stb_image.h"

namespace KENG {
    class MapLabel { // Will be as uniform as possible in order to use for both realm names and province names
        private:
            llui id = 0;            // Tied to either a province or realm, based on the data container
            bool dirty = false;
            f32 x               = 0.0f;
            f32 y               = 0.0f;
            f32 width           = 0.0f;
            f32 height          = 0.0f;
            f32 theta           = 0.0f; // Rotation of bounding box
        public:
            MapLabel(llui id, f32 x, f32 y, f32 width, f32 height, f32 theta = 0.0f);
            ~MapLabel(void);

            bool IsDirty(void); // Check if dirty

            void Mark(void);    // Mark as dirty
            void Unmark(void);  // mark as clean

            llui Id(void) const ;

            f32 X(void) const ;
            f32 Y(void) const ;
            
            f32 Width(void) const ;
            f32 Height(void) const ;
            
            f32 Theta(void) const ;
    }; // Map label class

    typedef struct ProvincePixelData {
        std::vector<ui32> pixels;
        i32 minX = UINT32_MAX;
        i32 minY = UINT32_MAX;
        i32 maxX = 0;
        i32 maxY = 0;
    } PPD; // Province pixel data for raw geometry boundaries

    typedef struct LabelRect {
        i32 x = 0;
        i32 y = 0;
        i32 width = 0;
        i32 height = 0;
        i32 area = 0;
    } LabelRect; // Label rectangle data

    // Build once, province labels are static, bound by province shape
    std::vector<MapLabel> BuildProvinceMapLabelsCPU(const char* fpath, ProvinceRegistry& preg, FT_Face face);

    void RenderProvinceMapLabels(OGL_Object& obj, const std::vector<MapLabel> labels, ProvinceRegistry& preg, glm::vec3 color, std::map<char, OGL_Character>& characters);
} // KENG