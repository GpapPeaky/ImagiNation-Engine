#pragma once

#include "KENG_EffectParser.hpp"

namespace KENG { // TODO: Call ts
    std::unordered_map<EffectParserKeys, std::string> CreateEffectParserKeys(void) {
        return {
                {EffectParserKeys::CONTROL_PER, "CTRL%"},
                {EffectParserKeys::CONTROL_FLAT, "CTLR"},
                {EffectParserKeys::TAX_PER, "TAX%"},
                {EffectParserKeys::TAX_FLAT, "TAX"},
                {EffectParserKeys::GOODS_GAIN_FLAT, "GOODS_GAIN"},
                {EffectParserKeys::GOODS_GAIN_PER, "GOODS_GAIN%"},

                {EffectParserKeys::BUILDING_COST_PER, "BUILD_COST%"},
                {EffectParserKeys::DEV_COST_PER, "DEV_COST%"},

                {EffectParserKeys::TAX_GAIN_PER_TAX_DEV_FLAT, "TAX_PER_TAX_DEV"},
                {EffectParserKeys::TAX_GAIN_PER_TAX_DEV_PER, "TAX_PER_TAX_DEV%"},
                {EffectParserKeys::GOODS_GAIN_PER_PROD_DEV_FLAT, "GOODS_GAIN_PER_PROD_DEV"},
                {EffectParserKeys::GOODS_GAIN_PER_PROD_DEV_PER, "GOODS_GAIN_PER_PROD_DEV%"},

                {EffectParserKeys::MANPOWER_PER, "MNPOWER%"},
                {EffectParserKeys::MANPOWER_FLAT, "MNPOWER"},
                {EffectParserKeys::MANPOWER_REINFORMENT_PER, "UNIT_MNPOWER_REINFORCE%"},
                {EffectParserKeys::RECRUIT_COST_PER, "UNIT_RECRUIT_COST%"},
                {EffectParserKeys::RECRUIT_TIME_PER, "UNIT_RECRUIT_TIME%"},
            
                /* Applies to leaders not units directly */
                {EffectParserKeys::LEADER_LIGHT_ATTACK_FLAT, "LEADER_LATT"},
                {EffectParserKeys::LEADER_LIGHT_ATTACK_PER, "LEADER_LATT%"},
                {EffectParserKeys::LEADER_HEAVY_ATTACK_FLAT, "LEADER_HATT"},
                {EffectParserKeys::LEADER_HEAVY_ATTACK_PER, "LEADER_HATT%"},
                {EffectParserKeys::LEADER_CASUALTIES_DEALT_PER, "LEADER_CAS_DEALT%"},
                {EffectParserKeys::LEADER_LIGHT_DEFENSE_PER, "LEADER_LDEF%"},
                {EffectParserKeys::LEADER_LIGHT_DEFENSE_FLAT, "LEADER_LDEF"},
                {EffectParserKeys::LEADER_HEAVY_DEFENSE_PER, "LEADER_HDEF%"},
                {EffectParserKeys::LEADER_HEAVY_DEFENSE_FLAT, "LEADER_HDEF"},
                {EffectParserKeys::LEADER_MORALE_REINFORCEMENT_PER, "LEADER_MORALE_REINFORCE%"},
                {EffectParserKeys::LEADER_SPEED_PER, "LEADER_SPD%"},
                {EffectParserKeys::LEADER_TACTICS_FLAT, "LEADER_TACTICS"},
        
                {EffectParserKeys::LIGHT_UNIT_LIGHT_ATTACK_FLAT,"UNIT_LUNIT_LATT" },
                {EffectParserKeys::LIGHT_UNIT_LIGHT_ATTACK_PER,"UNIT_LUNIT_LATT%" },
                {EffectParserKeys::LIGHT_UNIT_HEAVY_ATTACK_FLAT,"UNIT_LUNIT_HATT" },
                {EffectParserKeys::LIGHT_UNIT_HEAVY_ATTACK_PER,"UNIT_LUNIT_HATT%" },
                {EffectParserKeys::LIGHT_UNIT_CASUALTIES_DEALT_PER,"UNIT_LUNIT_CAS_DEALT%" },
                {EffectParserKeys::LIGHT_UNIT_LIGHT_DEFENSE_PER,"UNIT_LUNIT_LDEF%" },
                {EffectParserKeys::LIGHT_UNIT_LIGHT_DEFENSE_FLAT,"UNIT_LUNIT_LDEF" },
                {EffectParserKeys::LIGHT_UNIT_HEAVY_DEFENSE_PER,"UNIT_LUNIT_HDEF%" },
                {EffectParserKeys::LIGHT_UNIT_HEAVY_DEFENSE_FLAT,"UNIT_LUNIT_DEF" },
        
                {EffectParserKeys::HEAVY_UNIT_LIGHT_ATTACK_FLAT,"UNIT_HUNIT_LATT" },
                {EffectParserKeys::HEAVY_UNIT_LIGHT_ATTACK_PER,"UNIT_HUNIT_LATT%" },
                {EffectParserKeys::HEAVY_UNIT_HEAVY_ATTACK_FLAT,"UNIT_HUNIT_HATT" },
                {EffectParserKeys::HEAVY_UNIT_HEAVY_ATTACK_PER,"UNIT_HUNIT_HATT%" },
                {EffectParserKeys::HEAVY_UNIT_CASUALTIES_DEALT_PER,"UNIT_HUNIT_CAS_DEALT%" },
                {EffectParserKeys::HEAVY_UNIT_LIGHT_DEFENSE_PER,"UNIT_HUNIT_LDEF%" },
                {EffectParserKeys::HEAVY_UNIT_LIGHT_DEFENSE_FLAT,"UNIT_HUNIT_LDEF" },
                {EffectParserKeys::HEAVY_UNIT_HEAVY_DEFENSE_PER,"UNIT_HUNIT_HDEF%" },
                {EffectParserKeys::HEAVY_UNIT_HEAVY_DEFENSE_FLAT,"UNIT_HUNIT_HDEF" },
        
                {EffectParserKeys::AUX_UNIT_LIGHT_ATTACK_FLAT,"UNIT_AUNIT_LATT" },
                {EffectParserKeys::AUX_UNIT_LIGHT_ATTACK_PER,"UNIT_AUNIT_LATT%" },
                {EffectParserKeys::AUX_UNIT_HEAVY_ATTACK_FLAT,"UNIT_AUNIT_HATT" },
                {EffectParserKeys::AUX_UNIT_HEAVY_ATTACK_PER,"UNIT_AUNIT_HATT%" },
                {EffectParserKeys::AUX_UNIT_CASUALTIES_DEALT_PER,"UNIT_AUNIT_CAS_DEALT%" },
                {EffectParserKeys::AUX_UNIT_LIGHT_DEFENSE_PER,"UNIT_AUNIT_LDEF%" },
                {EffectParserKeys::AUX_UNIT_LIGHT_DEFENSE_FLAT,"UNIT_AUNIT_LDEF" },
                {EffectParserKeys::AUX_UNIT_HEAVY_DEFENSE_PER,"UNIT_AUNIT_HDEF%" },
                {EffectParserKeys::AUX_UNIT_HEAVY_DEFENSE_FLAT,"UNIT_AUNIT_HDEF" },
        
                {EffectParserKeys::ACCEPTED_CULTURAL_CONVERSION_COST_PER, "ACCEPTED_CULTURAL_CONVERSION_COST%"},
                {EffectParserKeys::NON_ACCEPTED_CULTURAL_CONVERSION_COST_PER, "CULTURAL_CONVERSION_COST%"},
                {EffectParserKeys::CULTURAL_ACCEPTANCE_COST_PER, "CULTURAL_ACCEPTANCE_COST%"},
                {EffectParserKeys::RELIGIOUS_CONVERSION_COST_PER, "RELIGIOUS_CONVERSION_COST%"},
                
                {EffectParserKeys::STABILITY_INCREASE_COST_PER, "STAB_COST%"}
        };
    }
} // KENG