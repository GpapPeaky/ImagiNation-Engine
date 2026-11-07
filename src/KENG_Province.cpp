#include "KENG_Province.hpp"

namespace KENG {
    Province::Province(llui id, std::string name, ui32 color) {
        this->id = id;
        this->name = name;
        this->color = color;
        this->dev = {1, 1, 1};
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
    
    Development& Province::Dev(void) { return dev; }
    const Development& Province::Dev(void) const { return dev; }

    namespace PROV {
        llui NULL_PROV_ID = 0xffffffff;
    }
}