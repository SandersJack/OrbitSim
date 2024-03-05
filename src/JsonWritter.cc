#include "JsonWritter.hh"

#include <cstdio>

#include "Planets.hh"
#include "RunManager.hh"

JsonWritter::JsonWritter(): fOutFilename("proton.json"){

    std::remove(fOutFilename);

    fJsonFile.open(fOutFilename, std::ios::app);

    if (!fJsonFile.is_open()) {
        std::cerr << "Error opening the file: " << fOutFilename << std::endl;
    } else {
        std::cout << "Saving to JSON file: " << fOutFilename << std::endl;
    }

    fjsonContent_ = "{\n  \"simulation\": {\n \"time_steps\": [";
}

JsonWritter *JsonWritter::fInstance = nullptr;

JsonWritter *JsonWritter::GetInstance() {
  if(!fInstance)
    fInstance = new JsonWritter();
  return fInstance;
};

void JsonWritter::addData(bool lastStep) {
    fjsonContent_ += "    {\n";
    fjsonContent_ += "      \"planets\": [\n";
    addCelestialObjects(fCelestialInfo.planets);
    fjsonContent_ += "      ],\n";
    fjsonContent_ += "      \"moons\": [\n";
    addCelestialObjects(fCelestialInfo.moons);
    fjsonContent_ += "      ],\n";
    fjsonContent_ += "      \"satellites\": [\n";
    addCelestialObjects(fCelestialInfo.satellites);
    fjsonContent_ += "      ]\n";
    if(lastStep){
        fjsonContent_ += "    }";
    } else {
        fjsonContent_ += "    },";
    }
    

}

template<typename T>
void JsonWritter::addCelestialObjects(const std::vector<T*>& celestialObjects){
    for (auto it = celestialObjects.begin(); it != celestialObjects.end(); ++it) {
        const auto& celestialObject = *it;
        fjsonContent_ += "    {";
        std::string name = celestialObject->GetName();
        fjsonContent_ += "      \"name\": \"" + name + "\",";
        fjsonContent_ += "      \"x\": " + std::to_string(celestialObject->GetPosition().GetX()) + ",";
        fjsonContent_ += "      \"y\": " + std::to_string(celestialObject->GetPosition().GetY()) + ",";
        fjsonContent_ += "      \"z\": " + std::to_string(celestialObject->GetPosition().GetY());
        fjsonContent_ += "    }";

        // Check if it's not the last element before adding a comma
        if (std::next(it) != celestialObjects.end()) {
            fjsonContent_ += ",";
        }

        fjsonContent_ += "\n";
    }
}

void JsonWritter::saveToFile(std::string content) {
    if (fJsonFile.is_open()) {
            fJsonFile << content;
    } else {
        std::cerr << "Error: File not open for writing." << std::endl;
    }
}

void JsonWritter::EndStep(){
    saveToFile(fjsonContent_);
    fjsonContent_ = "";
}

void JsonWritter::SaveStep(bool lastStep) {
    std::cout << "Save Step" << std::endl;
    addData(lastStep);
    EndStep();
}

void JsonWritter::EndRun() {
    fjsonContent_ = "\n  ]\n}\n}";
    saveToFile(fjsonContent_);
    fJsonFile.close();
}