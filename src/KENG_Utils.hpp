#pragma once

typedef long long unsigned int llui;
typedef long unsigned int lui;
typedef unsigned char ui8;
typedef unsigned int ui32;
typedef int i32;
typedef float f32;

#include <iostream> // For args
#include <fstream>

#include "../OGL.hpp"

namespace KENG::Utils {
    ui32 PackRGB(ui8 r, ui8 g, ui8 b);
    std::array<float, 3> UnpackRGB_01(ui32 rgb);
    std::array<ui8, 3> UnpackRGB_0255(ui32 rgb);
    std::array<ui8, 3> HSV2RGB(f32 h, f32 s, f32 v);

    extern std::ofstream LOG_RLM;  // Realm logging file
    extern std::ofstream LOG_PROV; // Province logging file
    extern std::ofstream LOG_DEV;  // Dev logging file
} // Keng/Utils