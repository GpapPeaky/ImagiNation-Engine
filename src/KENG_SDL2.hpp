#pragma once

#include "KENG_HoverController.hpp"
#include "../OGL.hpp"

namespace KENG::SDL2 {
    void HandleEvents(bool& quit, OGL_Controller* ctrl, HoverController& provCtrl, ProvinceRegistry& preg, RealmRegistry& rreg, OGL_Object* provMap); // Custom event handling function
}