#include "KENG_Culture.hpp"

namespace KENG {
    Culture::Culture(void) {}

    Culture::~Culture(void) {}

    ui32 Culture::Color(void) const {
        return color;
    }

    std::string Culture::Name(void) const {
        return name;
    }

    std::string Culture::Adj(void) const {
        return adjective;
    }

    ui32 Culture::Id(void) const {
        return id;
    }
} // KENG