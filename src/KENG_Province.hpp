#pragma once

#include "KENG_Utils.hpp"
#include "KENG_Dev.hpp"

#include <string>
#include <array>

namespace KENG {
    class Province {
        private:
            llui id                               = 0;
            std::string name                      = "PROV";
            ui32 color                            = 0xffffff;
            Development dev                       = { 1, 1, 1 }; // Read from History/dev.txt
            // TODO: Terrain type               // Read from Map/terrain.txt
            // TODO: Buildings                  // Read from History/buildings.txt
        public:
            Province(llui id, std::string name, ui32 color);
            ~Province(void);

            std::string& Name(void);
    
            llui Id(void) const ;

            ui32 Color(void) const ;

            Development& Dev(void);
            const Development& Dev(void) const;
    }; // Province class

    namespace PROV {
        extern llui NULL_PROV_ID;
    }
} // Kallergis engine namespace