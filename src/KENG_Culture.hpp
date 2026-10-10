#pragma once

#include "KENG_Utils.hpp"

namespace KENG {
    class Culture {
        private:
            ui32 id;                /* Culture id, also used as a passive effect id in the effect registry */
            std::string name;       /* Name of culture */
            std::string adjective;  /* Adjective */
            ui32 color;             /* Color of culture */
        public:
            Culture(void);

            ~Culture(void);

            ui32 Color(void) const ;

            std::string Name(void) const ;

            std::string Adj(void) const ;

            ui32 Id(void) const ;
    }; // Culture
} // KENG