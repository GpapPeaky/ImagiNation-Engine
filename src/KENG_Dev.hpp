#pragma once

#include "KENG_Utils.hpp"

namespace KENG {
    class Development {
        public:
            ui32 adm;           // Adminstrative, tax increase
            ui32 prd;           // Production, goods production
            ui32 mil;           // Military, manpower gain

        Development(ui32 adm, ui32 prd, ui32 mil);
        ~Development(void);
    }; // Province development class
} // KENG