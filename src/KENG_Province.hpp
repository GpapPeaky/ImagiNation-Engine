#pragma once

#include "KENG_Utils.hpp"

#include <string>
#include <array>

namespace KENG {
    class Province {
        private:
            llui id                               = 0;
            std::string name                      = "PROV";
            ui32 color                            = 0xffffff;
            // TODO: Terrain type               // Read from Map/terrain.txt
            // TODO: Development                // Read from History/dev.txt
            // TODO: Buildings                  // Read from History/buildings.txt
        public:
            Province(llui id, std::string name, ui32 color);
            ~Province(void);

            std::string& Name(void);
    
            llui Id(void) const ;

            ui32 Color(void) const ;
    }; // Province class

    namespace PROV {
        extern llui NULL_PROV_ID;
    }
} // Kallergis engine namespace