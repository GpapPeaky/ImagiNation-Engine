#pragma once

#include "KENG_HoverController.hpp"
#include "../OGL.hpp"

namespace KENG::SDL2 {
    const float MAX_ZOOM = 144.0f;
    const float MIN_ZOOM = 2.0f;

    void Zoom(float direction);

    void HandleEvents(bool& quit, OGL_Controller* ctrl, HoverController& provCtrl, ProvinceRegistry& preg, RealmRegistry& rreg, OGL_Object* provMap); // Custom event handling function
}