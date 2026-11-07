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

    std::array<ui8, 3> HSV2RGB(f32 h, f32 s, f32 v) {
        float c = v * s, x = c * (1.f - std::fabs(std::fmod(h / 60.f, 2.f) - 1.f)), m = v - c;
        float r = 0, g = 0, b = 0;
        if (h < 60) {
            r = c;
            g = x;
        }

        else if (h < 120) { 
            r = x;
            g = c;
        }

        return {
            static_cast<ui8>((r + m) * 255),
            static_cast<ui8>((g + m) * 255),
            static_cast<ui8>((b + m) * 255)
        };
    }

    std::ofstream LOG_PROV = std::ofstream("logs/prov.log");
    std::ofstream LOG_RLM = std::ofstream("logs/rlm.log");
    std::ofstream LOG_DEV = std::ofstream("logs/dev.log");
}
