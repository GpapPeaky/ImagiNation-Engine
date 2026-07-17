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
                b < 0 || b > 255)
            {
                Utils::LOG_RLM << "Invalid RGB value: " << line << '\n';
                continue;
            }
    
            ui32 color = Utils::PackRGB(
                static_cast<ui8>(r),
                static_cast<ui8>(g),
                static_cast<ui8>(b)
            );
    
            Realm rlm{id, name, color};
    
            colorToId.emplace(color, id);
            realms.push_back(std::move(rlm));
    
            ++id;
        }
    }

    void RealmRegistry::ReadOwnerFile(void) {
        // TODO:
    }

    void RealmRegistry::Print(void) {
        for (Realm realm : realms) {
            Utils::LOG_RLM << realm.Id() << " " << realm.Name() << " " << realm.Color() << " " << " " << std::endl;
        }
    }
}