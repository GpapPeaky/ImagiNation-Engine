#include "KENG_ProvinceController.hpp"

namespace KENG {
    ProvinceController::ProvinceController(void) {}

    ProvinceController::~ProvinceController(void) {}

    Province& ProvinceController::GetHoveredProvince(ProvinceRegistry& pr, OGL_Object& provMap) {
        std::array<ui8, 3> color = OGL_GetHoveredColourFromTexture(provMap.mat.texture, OGL_GetModel(provMap), OGL_RenderView);

        hoveredColor = Utils::PackRGB(color[0], color[1], color[2]);

        return pr.GetProvince(color);
    }

    Realm& ProvinceController::GetHoveredRealm(RealmRegistry& rr, OGL_Object& realmMaskMap) {
        std::array<ui8, 3> color = OGL_GetHoveredColourFromTexture(realmMaskMap.mat.texture, OGL_GetModel(realmMaskMap), OGL_RenderView);

        hoveredColor = Utils::PackRGB(color[0], color[1], color[2]);

        return rr.GetRealm(hoveredColor);
    }
}