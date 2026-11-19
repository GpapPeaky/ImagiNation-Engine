#pragma once

#include "KENG_Utils.hpp"

namespace KENG {
    class City {
        public:
            i32 cityObjX = 0;
            i32 cityObjY = 0;

            City(i32 x, i32 y);
            ~City();
    }; // City class
} // KENG