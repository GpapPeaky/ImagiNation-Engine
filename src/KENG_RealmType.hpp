#pragma once

#include <unordered_map>
#include "KENG_Culture.hpp"

namespace KENG { 
    typedef enum class RealmType {
        GRAND_REPUBLIC,
        REPUBLIC,
        MERCHANT_REPUBLIC,
        PEASANT_REPUBLIC,
        REPUBLICAN_DICTATORSHIP,
        PIRATE_REPUBLIC,

        /* SPECIAL REPUBLIC, ALLOWED ONLY FOR THE EYE ISLAND NATIONS, PRIMARY CULTURE */
        EYE_ISLE_REPUBLIC,
        
        EMPIRE,
        KINGDOM,
        ARCHDUCHY,
        DUCHY,
        MARCH,
        COUNTY,
        
        ARCHBISHOPRY,
        BISHOPRY,
        
        /* ORDERS, ESTABLISHED BY BISHOPS/ARCHBISHOPS AND PAPACY */
        HOLY_ORDER,
        MONASTIC_ORDER,
        MILITARY_ORDER,
        
        /* Holy Hierarchiate */
        ELECTIVE_EMPIRE,                /* EMPEROR */
        ELECTORATE,                     /* REALM THAT ELECTS */
        
        /* SPECIAL IMPERIAL PARTNER, CANNOT 
        BE DECLARED UPON AND CANNOT DECLARE WARS 
        ONLY GRANTED TO 1 PROVINCE NATIONS,
        ELSE DEFAULTS TO COUNTY */
        FREE_IMPERIAL_CITY,
        
        /* Papacy */
        PAPACY,
        
        /* TRIBES */
        TRIBAL_KINGDOM,
        TRIBE,
        
        /* COLONIAL, ESTABLISHED BY ANYONE EXCEPT 
            BISHOP/ARCHBISHOPS/PAPACY/
            MERCHANT REPUBLICS
            PIRATE REPUBLICS
        */
        COLONIAL_ADMINISTRATION,
        
        GRAND_FEDERATION,
        FEDERATION,
        
        /* Eastern kingdoms */
        CELESTIAL_EMPIRE,
        CELESTIAL_KINGDOM,
        
        HORDE,
        
        /* NON-COLONIAL ESTABLISHED BY PIRATE REPUBLICS/MERCHANT REPUBLICS*/
        TRADE_CITY,
        /* COLONIAL ESTABLISHED BY PIRATE REPUBLICS/MERCHANT REPUBLICS */
        CORPORATE_ADMINISTRATION,
    } RealmType;

    /* Map for base realm type names on display for accurate info, 
        if no cultural name is found we default here */
    std::unordered_map<RealmType, std::string> ReadBaseRealmTypeNameFile(void); 

    /* Map for realm type name on display per culture */
    extern std::unordered_map<
        Culture,
        std::unordered_map<RealmType, std::string>
    > CulturalRealmTypeNames;

    /* Map for realm type title of leader on display per culture */
    extern std::unordered_map<
        Culture,
        std::unordered_map<RealmType, std::string>
    > CulturalRealmLeaderTitleNames;

} // KENG