#include "KENG_HoverController.hpp"

namespace KENG {
    HoverController::HoverController(void) {}

    HoverController::~HoverController(void) {}

    void HoverController::GetHoveredProvince(ProvinceRegistry& pr, OGL_Object& provMap) {
        std::array<ui8, 3> color = OGL_GetHoveredColourFromTexture(provMap.mat.texture, OGL_GetModel(provMap), OGL_RenderView);

        hoveredProvince = pr.GetProvince(color);
    }

    void HoverController::GetHoveredRealm(RealmRegistry& rr, Province& hoveredProv) {
        llui provId = hoveredProv.Id();

        hoveredRealm = rr.GetOwnerOfProvince(provId);        
    }

    Province& HoverController::HoverProv(void) {
        return hoveredProvince;
    }

    Realm& HoverController::HoverRealm(void) {
        return hoveredRealm;
    }

    void HoverController::SetHoveredProvinceAsClicked(void) {
        clickedProvince = hoveredProvince;
    }

    void HoverController::ResetClickedProvince(void) {
        static Province nullProvince{PROV::NULL_PROV_ID, "nullprov", 0xffffff}; // This is some sort of discoverable province getter func
        clickedProvince = nullProvince;
    }

    Province& HoverController::ClickedProvince(void) {
        return clickedProvince;
    }
}