#pragma once

#include "KENG_Realm.hpp"

#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>

namespace KENG {
    class RealmRegistry {
        private:
            std::unordered_map<ui32, llui> colorToId;
            std::vector<Realm> realms; // Realms registry
        public:
            void ReadRealmFile(void);   // Realm creation, province assignment, AssignProvinces() function is a Realm Method

            void Print(ProvinceRegistry& pr); // Prints the generated entries, debug only.

            std::vector<Realm>& Realms(void);

            Realm& GetRealm(llui id);
            Realm& GetRealm(std::array<ui8, 3> color);
    }; // Registry
}