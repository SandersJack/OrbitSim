#ifndef RunManager_H
#define RunManager_H 

#include "Vector3D.hh"

#include "Planets.hh"
#include "Moons.hh"
#include "Satellite.hh"
#include "Earth.hh"
#include "Mars.hh"
#include "Moon.hh"

#include "Hdf5IO.hh"
#include "WebIO.hh"
#include "OrbitHelper.hh"

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <utility>




class RunManager {

    public:
        RunManager();
        static RunManager *GetInstance();
        void Test() {std::cout << "HERE"<<std::endl;}

        void Init();
        void Run();

        void SetTimeStep(double val) {fdt = val;}
        void SetStopTime(double val) {fStopTime = val;}

        void SetWebOut(bool val){ fWebOut = val;}

        std::vector<Planets *>  GetPlanetList() { return fPlanets;}
        std::vector<Moons *>  GetMoonList() { return fMoons;}
        std::vector<Satellite *>  GetSatelliteList() { return fSatellites;}

    private:
        Moon *fMoon;

        double fdt;
        double fStopTime;

        std::vector<Planets *> fPlanets;
        std::vector<std::string> fPlanetList;
        std::map<std::string, std::vector<double>> fPlanetAtrMap;

        std::vector<Moons *> fMoons;
        std::map<std::string, std::string> fMoonMap;

        std::vector<Satellite *> fSatellites;
        std::map<std::string, std::vector<std::pair<std::string, std::string>>> fSatMap;
        std::map<std::string, std::vector<double>> fSatAtrMap;
        

        Hdf5IO *fIOManager;
        WebIO *fWebIOManager;
        
        OrbitHelper *fHelper;

        bool fWebOut{false};
};
#endif