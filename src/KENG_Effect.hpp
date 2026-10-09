#pragma once

#include "KENG_Utils.hpp"
#include "KENG_EffectParserKeys.hpp"

namespace KENG {
    class Effect {
        private:
            ui32 id;                     // Effect id
            ui32 durationInMonths;       // Duration in months
            // Access via EffectParserKeys as indeces to the vector
            std::array<f32, static_cast<std::size_t>(EffectParserKeys::EFFECT_KEY_COUNT)> vals = 
                    { 0.0f }; // Actual effect values postive -> adds, negative -> removes
        public:
            Effect(void);

            ~Effect(void);

            ui32 Id(void) const ;

            std::array<f32, static_cast<std::size_t>(EffectParserKeys::EFFECT_KEY_COUNT)> Values(void) const ; // Get the effect values

            // Change a value on an index
            void UpdateValue(std::size_t effectArrayIndex, f32 delta);

            // Set a value to something
            void SetValue(std::size_t effectArrayIndex, f32 value);

            // Get a value
            f32 ValueAt(std::size_t effectArrayIndex) const ;
    }; // Effect class
} // KENG