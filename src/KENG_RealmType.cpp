#include "KENG_RealmType.hpp"

namespace KENG {
    static const std::unordered_map<std::string, RealmType> realmTypeStringToType = {
        { "GRAND_REPUBLIC",          RealmType::GRAND_REPUBLIC },
        { "REPUBLIC",                RealmType::REPUBLIC },
        { "MERCHANT_REPUBLIC",       RealmType::MERCHANT_REPUBLIC },
        { "PEASANT_REPUBLIC",        RealmType::PEASANT_REPUBLIC },
        { "REPUBLICAN_DICTATORSHIP", RealmType::REPUBLICAN_DICTATORSHIP },
        { "PIRATE_REPUBLIC",         RealmType::PIRATE_REPUBLIC },
        { "EYE_ISLE_REPUBLIC",       RealmType::EYE_ISLE_REPUBLIC },
        { "EMPIRE",                  RealmType::EMPIRE },
        { "KINGDOM",                 RealmType::KINGDOM },
        { "ARCHDUCHY",               RealmType::ARCHDUCHY },
        { "DUCHY",                   RealmType::DUCHY },
        { "MARCH",                   RealmType::MARCH },
        { "COUNTY",                  RealmType::COUNTY },
        { "ARCHBISHOPRY",            RealmType::ARCHBISHOPRY },
        { "BISHOPRY",                RealmType::BISHOPRY },
        { "HOLY_ORDER",              RealmType::HOLY_ORDER },
        { "MONASTIC_ORDER",          RealmType::MONASTIC_ORDER },
        { "MILITARY_ORDER",          RealmType::MILITARY_ORDER },

        { "ELECTIVE_EMPIRE",         RealmType::ELECTIVE_EMPIRE },
        { "ELECTORATE",              RealmType::ELECTORATE },
        { "FREE_IMPERIAL_CITY",      RealmType::FREE_IMPERIAL_CITY },

        { "PAPACY",                  RealmType::PAPACY },

        { "TRIBAL_KINGDOM",          RealmType::TRIBAL_KINGDOM },
        { "TRIBE",                   RealmType::TRIBE },

        { "COLONIAL_ADMINISTRATION", RealmType::COLONIAL_ADMINISTRATION },

        { "GRAND_FEDERATION",        RealmType::GRAND_FEDERATION },
        { "FEDERATION",              RealmType::FEDERATION },

        { "CELESTIAL_EMPIRE",        RealmType::CELESTIAL_EMPIRE },
        { "CELESTIAL_KINGDOM",       RealmType::CELESTIAL_KINGDOM },

        { "HORDE",                   RealmType::HORDE },
        { "TRADE_CITY",              RealmType::TRADE_CITY },

        { "CORPORATE_ADMINISTRATION", RealmType::CORPORATE_ADMINISTRATION },
    };

    RealmType GetRealmTypeFromString(std::string typeName) {
        auto it = realmTypeStringToType.find(typeName);

        if (it == realmTypeStringToType.end()) {

            std::cerr << "Unknown realm type '" + typeName + "\n";

            return RealmType::UNKOWN_REALM_TYPE;
        }

        return it->second;
    }

    std::unordered_map<RealmType, std::string> ReadBaseReamTypeNameFile(void) {
        // TODO:
    }

} // KENG