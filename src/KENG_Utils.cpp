#include "KENG_Utils.hpp"

namespace KENG::Utils{
    ui32 PackRGB(ui8 r, ui8 g, ui8 b) {
        return (static_cast<ui32>(r) << 16) |
               (static_cast<ui32>(g) << 8)  | 
                static_cast<ui32>(b);
    }

    std::array<float, 3> UnpackRGB_01(ui32 rgb) {
        return {
            static_cast<float>((rgb >> 16) & 0xFF) / 255.0f, // R
            static_cast<float>((rgb >> 8)  & 0xFF) / 255.0f, // G
            static_cast<float>( rgb        & 0xFF) / 255.0f  // B
        };
    }

    std::array<ui8, 3> UnpackRGB_0255(ui32 rgb) {
        return {
            static_cast<ui8>((rgb >> 16) & 0xFF), // R
            static_cast<ui8>((rgb >> 8)  & 0xFF), // G
            static_cast<ui8>( rgb        & 0xFF)  // B
        };
    }

    std::ofstream LOG_PROV = std::ofstream("logs/prov.log");
    std::ofstream LOG_RLM = std::ofstream("logs/rlm.log");
}
