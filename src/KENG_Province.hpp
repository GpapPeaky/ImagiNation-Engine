#pragma once

#include <string>
#include <array>

namespace KENG {
    class Province {
        private:
            long long unsigned int population   = 0;
            std::string name                    = "PROV";
            std::array<unsigned char, 3> color    = {255, 255, 255};

        public:
            Province(void);
            ~Province(void);
    }; // Province class
    
} // Kallergis engine namespace