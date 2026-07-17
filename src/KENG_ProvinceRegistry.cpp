#include "KENG_ProvinceRegistry.hpp"

namespace KENG {
    ProvinceRegistry::ProvinceRegistry(void) {}

    ProvinceRegistry::~ProvinceRegistry(void) {}

    Province& ProvinceRegistry::GetProvince(std::array<ui8, 3> color) {
        ui32 packedRGB = Utils::PackRGB(color[0], color[1], color[2]);

        auto it = colorToId.find(packedRGB); // Just need to find a number, not an entire array!

        if (it != colorToId.end()) {
            return provinces[it->second - 1];
        }

        const llui KENG_NULLPROV_ID = 0xffffffff;
        static Province nullProvince{KENG_NULLPROV_ID, "nullprov", 0xffffff}; // This is some sort of discoverable province getter func
        return nullProvince;
    }

    void ProvinceRegistry::ReadProvinceFile(void) {
        const std::string fpath = "History/provinces/province_colors.txt";

        std::ifstream inputFile(fpath);
    
        if (!inputFile.is_open()) {
            Utils::LOG_PROV << "Failed to open province data file: " << fpath << '\n';
            return;
        }
    
        std::string line;
        while (std::getline(inputFile, line)) {
            // Skip empty lines
            if (line.empty()) continue;
    
            std::stringstream ss(line);
    
            llui id;
            std::string name;
            int r, g, b;

            if (!(ss >> id)) {
                Utils::LOG_PROV << "Malformed line: " << line << '\n';
                continue;
            }

            ss >> std::ws;  // Skip whitespace

            if (ss.get() != '"') {
                Utils::LOG_PROV << "Expected opening quote: " << line << '\n';
                continue;
            }

            std::getline(ss, name, '"');  // Read until the closing quote

            if (!(ss >> r >> g >> b)) {
                Utils::LOG_PROV << "Malformed RGB: " << line << '\n';
                continue;
            }
    
            ui32 color = Utils::PackRGB(
                static_cast<ui8>(r),
                static_cast<ui8>(g),
                static_cast<ui8>(b)
            );
    
            Province prov{id, name, color};
    
            colorToId.emplace(color, id);
            provinces.push_back(std::move(prov));
        }
    }
    
    Province& ProvinceRegistry::GetProvince(llui id) {
        // We are required to have the correct id here, intended to be used AFTER getting the correct id or maybe after colorto id matching
        return provinces[id];
    }

    void ProvinceRegistry::Print(void) {
        for (auto prov : provinces) {
            Utils::LOG_PROV << prov.Id() << " " << prov.Name() << std::endl;
        }
    }
}