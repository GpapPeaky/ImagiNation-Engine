#include "KENG_Dev.hpp"

namespace KENG {
    Development::Development(ui32 adm, ui32 prd, ui32 mil){
        this->adm = adm;
        this->prd = prd;
        this->mil = mil;
    }

    Development::~Development(void) {}
            
    ui32 Development::Total(void) const {
        return adm + prd + mil;
    }

} // KENG