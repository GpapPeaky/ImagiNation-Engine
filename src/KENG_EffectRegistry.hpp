#pragma once

#include "KENG_EffectParser.hpp"
#include "KENG_Effect.hpp"
#include "KENG_EffectType.hpp"

namespace KENG {
    class EffectRegistry {
        private:
            std::array<
                std::vector<Effect>,
                static_cast<std::size_t>(EffectType::EFFECT_TYPES)
            > effects;

            // Append effects to registry
            void AppendEffects(EffectType type, std::vector<Effect> effects);
        public:
            EffectRegistry(void);
            
            ~EffectRegistry(void);
    }; // Effect Registry, realms, research etc. dereferences ONCE, effects from here, by adding/removing

} // KENG