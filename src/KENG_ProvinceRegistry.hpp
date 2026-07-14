#pragma once

#include "KENG_Utils.hpp"
#include "KENG_Province.hpp"

#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>

namespace KENG {
    class ProvinceRegistry {
        private:
            std::unordered_map<ui32, llui> colorToId;
            std::vector<Province> provinces;
        public:
            ProvinceRegistry(void);
            ~ProvinceRegistry(void);

            void ReadProvinceFile(void); // One specific file History/provinces.kengdat -> Id is set inside the file, to get the index -> id - 1

            Province& GetProvince(std::array<ui8, 3> color);
            
            Province& GetProvince(llui id);

            void Print(void); // Prints the generated entries, debug only.
    };
}