#pragma once

#include "KENG_Utils.hpp"
#include "KENG_ProvinceRegistry.hpp"
#include "KENG_RealmRegistry.hpp"
#include "KENG_GPU.hpp"

namespace KENG {
    class HoverController {
        private:
            Province clickedProvince{PROV::NULL_PROV_ID, "nullprov", 0xffffff};
            Province hoveredProvince{PROV::NULL_PROV_ID, "nullprov", 0xffffff};
            Realm hoveredRealm{RLM::NULL_RLM_ID, "nullrlm", 0xffffff};
        public:
            HoverController(void);
            ~HoverController(void);

            void GetHoveredProvince(ProvinceRegistry& pr, OGL_Object& provMap);

            void GetHoveredRealm(RealmRegistry& rr, Province& hoveredProv);

            Province& HoverProv(void);
            Realm& HoverRealm(void);

            void SetHoveredProvinceAsClicked(void);

            void ResetClickedProvince(void);

            Province& ClickedProvince(void);
    };
}