#include "KENG_Realm.hpp"

namespace KENG {
    Realm::Realm(llui id, std::string name, ui32 color) {
        this->id = id;
        this->name = name;
        this->color = color;
    }

    Realm::~Realm(void) {}

    llui Realm::Id(void) const {
        return id;
    }

    ui32 Realm::Color(void) const {
        return color;
    }

    std::string& Realm::Name(void) {
        return name;
    }

    bool Realm::HasProvince(llui provID) {
        for (llui id : provinces) {
            if (id == provID) return true;
        }

        return false;
    }

    bool Realm::HasProvince(ProvinceRegistry& pr, ui32 color) {
        llui provID = pr.GetProvince(color).Id();

        return HasProvince(provID);
    }       

    void Realm::AddProvince(llui provID) {
        if (HasProvince(provID)) return; // Already has it

        provinces.push_back(provID);
    }

    void Realm::AddProvince(ProvinceRegistry& pr, ui32 color) {
        if (HasProvince(pr, color)) return; // Already has it
        
        llui provID = pr.GetProvince(color).Id();

        provinces.push_back(provID);
    }
            
    void Realm::RemoveProvince(llui provID) {
        if (!HasProvince(provID)) return; // Doesn't have it

        auto it = std::find(provinces.begin(), provinces.end(), provID);

        if (it != provinces.end()) {
            provinces.erase(it);
        }
    }

    void Realm::RemoveProvince(ProvinceRegistry& pr, ui32 color) {
        if (!HasProvince(pr, color)) return; // Doesn't have it

        llui provID = pr.GetProvince(color).Id();

        RemoveProvince(provID);
    }

    void Realm::InitProvinces(void) {
        std::string strippedName = name;
        // Remove whitespaces
        strippedName.erase(
            std::remove(strippedName.begin(), strippedName.end(), ' '),
            strippedName.end()
        );

        std::string path = "History/owner/" + strippedName + ".txt";

        std::ifstream f(path);

        if (!f.is_open()) {
            Utils::LOG_RLM << "[Warning] No ownership file for the realm " << name << std::endl;
            return;
        }

        provinces.clear();

        llui provID;

        while (f >> provID) {
            provinces.push_back(provID);
        }

        f.close();
    }

    std::vector<llui>& Realm::Provinces(void) {
        return provinces;
    }

    namespace RLM {
        llui NULL_RLM_ID = 0xffffffff;
    }
}