#pragma once

#include "KENG_Utils.hpp"
#include "KENG_ProvinceRegistry.hpp"

#include <string>
#include <vector>
#include <algorithm>

namespace KENG {
    class Realm {
        private:
            llui id = 0;
            std::vector<llui> provinces; // Province id's further data is taken from the registry
            ui32 color       = 0xffffff; // Realm color
            std::string name = "REALM"; 
            // TODO: type
        public:
            Realm(llui id, std::string name, ui32 color);

            ~Realm(void);

            llui Id(void) const ;

            ui32 Color(void) const ;

            std::string& Name(void);

            bool HasProvince(llui provID);      // Check if the realm own a province of specific id
            bool HasProvince(ProvinceRegistry& pr, ui32 color);       // Check if the realm own a province of specific color

            void AddProvince(llui provID);  // Add province of specific id
            void AddProvince(ProvinceRegistry& pr, ui32 color);  // Add province of specific color
            
            void RemoveProvince(llui provID); // Remove province of specific id
            void RemoveProvince(ProvinceRegistry& pr, ui32 color); // Remove province of specific color

            void InitProvinces(void); // Assign province ids, read from file realms/ownership/realmName.txt

            std::vector<llui>& Provinces(void);
    };

    namespace RLM {
        extern llui NULL_RLM_ID;
    }
}