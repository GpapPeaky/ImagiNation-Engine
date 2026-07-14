#pragma once

#include "KENG_Utils.hpp"

#include <string>
#include <vector>

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

            ui32 Color(void);

            std::string Name(void);

            void AssignProvinces(void); // Assign province ids, read from file
    };
}