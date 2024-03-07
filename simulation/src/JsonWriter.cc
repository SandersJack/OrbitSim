#include "JsonWriter.hh"

#include <cstdio>

#include "Planets.hh"
#include "RunManager.hh"

JsonWriter::JsonWriter(): fOutFilename("proton.json"){

    std::remove(fOutFilename);

    fJsonFile.open(fOutFilename, std::ios::app);

    if (!fJsonFile.is_open()) {
        std::cerr << "[JsonWriter] Error opening the file: " << fOutFilename << std::endl;
    } else {
        std::cout << "[JsonWriter] Saving to JSON file " << fOutFilename << std::endl;
    }

    fjsonContent_ = "{\n  \"simulation\": {\n \"time_steps\": [";
}

JsonWriter *JsonWriter::fInstance = nullptr;

JsonWriter *JsonWriter::GetInstance() {
  if(!fInstance)
    fInstance = new JsonWriter();
  return fInstance;
};

void JsonWriter::addData(double time, bool lastStep) {
    fjsonContent_ += "    {\n";
    fjsonContent_ += "      \"time\": " + std::to_string(time) + ",\n";
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
void JsonWriter::addCelestialObjects(const std::vector<T*>& celestialObjects){
    for (auto it = celestialObjects.begin(); it != celestialObjects.end(); ++it) {
        const auto& celestialObject = *it;
        fjsonContent_ += "    {";
        std::string name = celestialObject->GetName();
        fjsonContent_ += "      \"name\": \"" + name + "\",";
        fjsonContent_ += "      \"x\": " + std::to_string(celestialObject->GetPosition().GetX()) + ",";
        fjsonContent_ += "      \"y\": " + std::to_string(celestialObject->GetPosition().GetY()) + ",";
        fjsonContent_ += "      \"z\": " + std::to_string(celestialObject->GetPosition().GetZ());
        fjsonContent_ += "    }";

        // Check if it's not the last element before adding a comma
        if (std::next(it) != celestialObjects.end()) {
            fjsonContent_ += ",";
        }

        fjsonContent_ += "\n";
    }
}

void JsonWriter::saveToFile(std::string content) {
    if (fJsonFile.is_open()) {
            fJsonFile << content;
    } else {
        std::cerr << "[JsonWriter] Error: File not open for writing." << std::endl;
    }
}

void JsonWriter::EndStep(){
    saveToFile(fjsonContent_);
    fjsonContent_ = "";
}

void JsonWriter::SaveStep(double time, bool lastStep) {
    addData(time, lastStep);
    EndStep();
}

void JsonWriter::EndRun() {
    fjsonContent_ = "\n  ]\n}\n}";
    saveToFile(fjsonContent_);
    fJsonFile.close();
}