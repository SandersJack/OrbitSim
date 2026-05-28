#include "Hdf5IO.hh"

#include "Planets.hh"
#include "Moons.hh"
#include "Satellite.hh"
#include "RunManager.hh"

#include <highfive/H5File.hpp>
#include <highfive/H5DataSet.hpp>
#include <highfive/H5DataSpace.hpp>

#include <iostream>

Hdf5IO::Hdf5IO(): fOutFilename("proton.h5") {

}

Hdf5IO *Hdf5IO::GetInstance() {
    static Hdf5IO instance;
    return &instance;
}

void Hdf5IO::Init() {
    std::cout << "[Hdf5IO] Writing to " << fOutFilename << std::endl;
}

void Hdf5IO::InitBranches() {
    for (Planets *p : RunManager::GetInstance()->GetPlanetList()) {
        fPlanetNames.push_back(p->GetName());
        std::cout << "[Hdf5IO] " << p->GetName() << " IO Initialised" << std::endl;
    }
    for (Moons *m : RunManager::GetInstance()->GetMoonList()) {
        fMoonNames.push_back(m->GetName());
        std::cout << "[Hdf5IO] " << m->GetName() << " IO Initialised" << std::endl;
    }
    for (Satellite *s : RunManager::GetInstance()->GetSatelliteList()) {
        fSatelliteNames.push_back(s->GetName());
        std::cout << "[Hdf5IO] " << s->GetName() << " IO Initialised" << std::endl;
    }
}

void Hdf5IO::SaveStep(double time) {
    fTime.push_back(time);

    for (Planets *p : RunManager::GetInstance()->GetPlanetList()) {
        Vector3D pos = p->GetPosition();
        Vector3D vel = p->GetVelocity();
        fPositions["Planets"][p->GetName()].push_back({pos.GetX(), pos.GetY(), pos.GetZ()});
        fVelocities["Planets"][p->GetName()].push_back({vel.GetX(), vel.GetY(), vel.GetZ()});
    }
    for (Moons *m : RunManager::GetInstance()->GetMoonList()) {
        Vector3D pos = m->GetPosition();
        Vector3D vel = m->GetVelocity();
        fPositions["Moons"][m->GetName()].push_back({pos.GetX(), pos.GetY(), pos.GetZ()});
        fVelocities["Moons"][m->GetName()].push_back({vel.GetX(), vel.GetY(), vel.GetZ()});
    }
    for (Satellite *s : RunManager::GetInstance()->GetSatelliteList()) {
        Vector3D pos = s->GetPosition();
        Vector3D vel = s->GetVelocity();
        fPositions["Satellites"][s->GetName()].push_back({pos.GetX(), pos.GetY(), pos.GetZ()});
        fVelocities["Satellites"][s->GetName()].push_back({vel.GetX(), vel.GetY(), vel.GetZ()});
    }
}

void Hdf5IO::EndRun() {
    HighFive::File file(fOutFilename, HighFive::File::Truncate);

    file.createDataSet<double>("time", HighFive::DataSpace::From(fTime)).write(fTime);

    for (const auto &[group, bodies] : fPositions) {
        for (const auto &[name, track] : bodies) {
            const std::string path = "/" + group + "/" + name;
            file.createDataSet<double>(path + "/position",
                                       HighFive::DataSpace::From(track)).write(track);
            const auto &vel = fVelocities.at(group).at(name);
            file.createDataSet<double>(path + "/velocity",
                                       HighFive::DataSpace::From(vel)).write(vel);
        }
    }

    std::cout << "[Hdf5IO] Wrote " << fTime.size() << " steps to " << fOutFilename << std::endl;
}
