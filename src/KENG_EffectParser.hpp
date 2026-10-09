#pragma once

#include "KENG_Utils.hpp"
#include "KENG_Effect.hpp"

namespace KENG {
    class EffectParser {
        private:
            std::unordered_map<EffectParserKeys, std::string> keys;
            
        public:
            EffectParser();
            ~EffectParser();

            // Return the keys
            std::unordered_map<EffectParserKeys, std::string> Keys(void) const ;
            
            // Check if key
            bool IsKey(std::string key) const ;
    }; // Effect parser object
} // KENG