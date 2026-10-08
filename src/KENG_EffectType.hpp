#pragma once

#include "KENG_Utils.hpp"

namespace KENG {
    enum class EffectType : std::size_t {
        RESEARCH_PASSIVE,
        MISSION_AFTEREFFECT,
        CULTURE_PASSIVE,
        RELIGION_PASSIVE,
        EVENTS_AFTEREFFECT,
        IDEAS_PASSIVE,
        DESCISIONS_AFTEREFFECT,
        REALM_TYPE,
        HERITAGE_PASSIVE,

        EFFECT_TYPES
    }; // Effect type enumerators, to calssify in effects registry, will be used as an index in the registry
} // KENG