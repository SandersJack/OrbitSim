#ifndef JsonWriter_H
#define JsonWriter_H 1 

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "Planets.hh"
#include "Moons.hh"
#include "Satellite.hh"

struct CelestialInfo {
    std::vector<Planets*> planets;
    std::vector<Moons*> moons;
    std::vector<Satellite*> satellites;
};

class JsonWriter {
    
    public:
        JsonWriter();

        static JsonWriter *GetInstance();

        void Init();
        void SaveStep(double time, bool lastStep);
        void EndStep();
        void EndRun();

        void addData(double time, bool lastStep);

        template<typename T>
        void addCelestialObjects(const std::vector<T*>& celestialObjects);
        void saveToFile(std::string content);

        void SetFilename(const char *val) {fOutFilename = val;}

        void SetCelestialInfo(CelestialInfo val) {fCelestialInfo = val;}

    private:
        static JsonWriter *fInstance;

        const char *fOutFilename;

        std::string fjsonContent_;

        CelestialInfo fCelestialInfo;

        std::ofstream fJsonFile;
};

#endif