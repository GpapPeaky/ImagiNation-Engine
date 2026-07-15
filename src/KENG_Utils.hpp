#pragma once

typedef long long unsigned int llui;
typedef unsigned char ui8;
typedef unsigned int ui32;

#include <iostream> // For args
#include <fstream>

#include "../OGL.hpp"

namespace KENG::Utils {
    ui32 PackRGB(ui8 r, ui8 b, ui8 g);
    std::array<float, 3> UnpackRGB_01(ui32 rgb);
    std::array<ui8, 3> UnpackRGB_0255(ui32 rgb);

    extern std::ofstream LOG_RLM;  // Realm logging file
    extern std::ofstream LOG_PROV; // Proving logging file
} // Keng/Utils