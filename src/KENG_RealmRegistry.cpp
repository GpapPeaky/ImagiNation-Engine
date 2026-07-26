#include "KENG_RealmRegistry.hpp"

namespace KENG {
    void RealmRegistry::ReadRealmFile(void) {
        const std::string fpath = "History/realms/realms.txt";

        std::ifstream inputFile(fpath);
    
        if (!inputFile.is_open()) {
            Utils::LOG_RLM << "Failed to open realm data file: " << fpath << '\n';
            return;
        }
    
        llui id = 0;
        std::string line;
        while (std::getline(inputFile, line)) {
            // Skip empty lines
            if (line.empty())
                continue;
    
            if (line[0] == '#') continue; // Comment

            std::stringstream ss(line);
            //
            int r, g, b;
            std::string realmType;
            std::string name;
    
            // Read RGB and realm type
            if (!(ss >> r >> g >> b >> realmType)) {
                Utils::LOG_RLM << "Malformed line: " << line << '\n';
                continue;
            }
    
            // Read the rest of the line as the realm name
            std::getline(ss >> std::ws, name);
    
            // Remove surrounding quotes if present
            if (name.size() >= 2 &&
                name.front() == '"' &&
                name.back() == '"')
            {
                name = name.substr(1, name.size() - 2);
            }
    
            // Validate RGB
            if (r < 0 || r > 255 ||
                g < 0 || g > 255 ||
                b < 0 || b > 255) {
                Utils::LOG_RLM << "Invalid RGB value: " << line << '\n';
                continue;
            }
    
            ui32 color = Utils::PackRGB(
                static_cast<ui8>(r),
                static_cast<ui8>(g),
                static_cast<ui8>(b)
            );
    
            Realm rlm{id, name, color};
            rlm.InitProvinces(); // Read from here, initialisation
    
            colorToId.emplace(color, id);
            realms.push_back(std::move(rlm));
    
            ++id;
        }
    }

    void RealmRegistry::Print(ProvinceRegistry& pr) {
        for (Realm realm : realms) {
            Utils::LOG_RLM << realm.Id() << " " << realm.Name() << " " << realm.Color() << " " << " " << std::endl;
            for (llui provID : realm.Provinces()) {

                Province& prov = pr.GetProvince(provID - 1);
                Utils::LOG_RLM << "\t" << provID << " " << prov.Name() << " " << prov.Color() << std::endl;
            }
        }
    }

    std::vector<Realm>& RealmRegistry::Realms(void) {
        return realms;
    }

    Realm& RealmRegistry::GetRealm(llui id) {
        return realms[id];
    }

    Realm& RealmRegistry::GetRealm(std::array<ui8, 3> color) {
        ui32 packedRGB = Utils::PackRGB(color[0], color[1], color[2]);

        auto it = colorToId.find(packedRGB); // Just need to find a number, not an entire array!

        if (it != colorToId.end()) {
            return realms[it->second];
        }

        const llui KENG_NULLRLM_ID = 0xffffffff;
        static Realm nullRealm{KENG_NULLRLM_ID, "nullrlm", 0xffffff}; // This is some sort of discoverable province getter func
        return nullRealm;
    }

    Realm& RealmRegistry::GetOwnerOfProvince(llui provId) {
        for (Realm& rlm : realms) {
            for (llui id : rlm.Provinces()) {
                if (id == provId) return rlm;
            }
        }

        /* Fallback */
        const llui KENG_NULLRLM_ID = 0xffffffff;
        static Realm nullRealm{KENG_NULLRLM_ID, "nullrlm", 0xffffff}; // This is some sort of discoverable province getter func
        return nullRealm;
    }
}