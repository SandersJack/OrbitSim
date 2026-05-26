#ifndef Hdf5IO_H
#define Hdf5IO_H

#include <string>
#include <vector>
#include <array>
#include <unordered_map>

class Hdf5IO {

    public:
        Hdf5IO();

        static Hdf5IO *GetInstance();

        void Init();
        void InitBranches();
        void SaveStep(double time);
        void EndRun();

        void SetFilename(const std::string &val) { fOutFilename = val; }

    private:
        static Hdf5IO *fInstance;

        std::string fOutFilename;

        std::vector<double> fTime;

        // group ("Planets" / "Moons" / "Satellites") -> body name -> per-step position/velocity
        using Track = std::vector<std::array<double, 3>>;
        std::unordered_map<std::string, std::unordered_map<std::string, Track>> fPositions;
        std::unordered_map<std::string, std::unordered_map<std::string, Track>> fVelocities;

        std::vector<std::string> fPlanetNames;
        std::vector<std::string> fMoonNames;
        std::vector<std::string> fSatelliteNames;
};

#endif
