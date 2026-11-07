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

        static Province nullProvince{PROV::NULL_PROV_ID, "nullprov", 0xffffff}; // This is some sort of discoverable province getter func
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

    void ProvinceRegistry::ReadDevelopmentFile(void) {
        const char* path = "History/dev.txt";
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "[ProvinceRegistry] Could not open " << path << "\n";
            return;
        }
    
        std::vector<bool> seen(provinces.size(), false);
        std::string line;
        size_t lineNo = 0;
        size_t applied = 0;
    
        while (std::getline(file, line)) {
            ++lineNo;
    
            size_t hash = line.find('#');
            if (hash != std::string::npos) line.erase(hash);
            if (line.find_first_not_of(" \t\r\n") == std::string::npos) continue;
    
            std::istringstream ss(line);
            long long id, adm, prd, mil;
            std::string extra;
    
            if (!(ss >> id >> adm >> prd >> mil) || (ss >> extra)) {
                std::cerr << "[Error] " << path << ":" << lineNo << ": malformed line\n";
                continue;
            }
            if (id < 1 || adm < 0 || prd < 0 || mil < 0) {
                std::cerr << "[Error] " << path << ":" << lineNo << ": negative or zero value\n";
                continue;
            }
    
            size_t index = static_cast<size_t>(id - 1);
            if (index >= provinces.size() || provinces[index].Id() != static_cast<llui>(id)) {
                std::cerr << "[Error] " << path << ":" << lineNo << ": unknown province id " << id << "\n";
                continue;
            }
    
            if (seen[index]) {
                std::cerr << "[Error] " << path << ":" << lineNo
                        << ": duplicate entry for province " << id << " (overwriting)\n";
            } else {
                seen[index] = true;
                ++applied;   // now counts distinct provinces
            }
    
            Development& d = provinces[index].Dev();
            d.adm = static_cast<ui32>(adm);
            d.prd = static_cast<ui32>(prd);
            d.mil = static_cast<ui32>(mil);
        }
    
        std::cout << "[Good] Applied development to " << applied
                << " / " << provinces.size() << " provinces\n";
    
        // List provinces that got no dev entry
        for (size_t i = 0; i < provinces.size(); ++i) {
            if (!seen[i]) {
                std::cout << "[Warning] No dev entry for province "
                        << provinces[i].Id() << " (" << provinces[i].Name() << ")\n";
            }
        }
    }
    
    Province& ProvinceRegistry::GetProvince(llui id) {
        // We are required to have the correct id here, intended to be used AFTER getting the correct id or maybe after colorto id matching
        return provinces[id];
    }

    const std::vector<Province>& ProvinceRegistry::Provinces(void) const {
        return provinces;
    }

    void ProvinceRegistry::Print(void) {
        for (auto prov : provinces) {
            Utils::LOG_PROV << prov.Id() << " " << prov.Name() << " " << prov.Color() << "\n";
        }
    }

    llui ProvinceRegistry::Count(void) {
        return provinces.size();
    }
}