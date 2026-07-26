#include "KENG_Province.hpp"

namespace KENG {
    Province::Province(llui id, std::string name, ui32 color) {
        this->id = id;
        this->name = name;
        this->color = color;
    }

    Province::~Province(void) {}

    std::string& Province::Name(void) {
        return name;
    }
    
    llui Province::Id(void) const {
        return id;
    }

    ui32 Province::Color(void) const {
        return color;
    }

    namespace PROV {
        llui NULL_PROV_ID = 0xffffffff;
    }
}