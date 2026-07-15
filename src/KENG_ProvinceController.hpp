#pragma once

#include "KENG_Utils.hpp"
#include "KENG_ProvinceRegistry.hpp"

namespace KENG {
    class ProvinceController {
        private:
            llui selectedProvinceId = 0;
            ui32 hoveredColor = 0xffffff;

        public:
            ProvinceController(void);
            ~ProvinceController(void);

            Province& GetHoveredProvince(ProvinceRegistry& pr, OGL_Object& provMap);
    };
}