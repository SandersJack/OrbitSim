#include "Hdf5IO.hh"

#include "Planets.hh"
#include "Moons.hh"
#include "Satellite.hh"
#include "RunManager.hh"

#include <highfive/H5File.hpp>
#include <highfive/H5DataSet.hpp>
#include <highfive/H5DataSpace.hpp>
#include <highfive/H5PropertyList.hpp>

#include <array>
#include <map>
#include <vector>
#include <iostream>

namespace {
// Steps buffered in memory before a chunk is flushed to disk. Bounds memory
// to ~CHUNK * nbodies * 48 bytes regardless of run length.
constexpr std::size_t CHUNK = 4096;
}

struct Hdf5IO::Impl {
    std::unique_ptr<HighFive::File> file;

    // Datasets keyed by full path ("time", "/Planets/Earth/position", ...).
    std::map<std::string, HighFive::DataSet> datasets;

    // Flush buffers, same keys as the datasets.
    std::vector<double> timeBuf;
    std::map<std::string, std::vector<std::array<double, 3>>> vecBuf;

    std::size_t rowsWritten = 0;   // rows already on disk
    std::size_t totalSteps = 0;

    HighFive::DataSet makeVecDataset(const std::string &path) {
        HighFive::DataSpace space({0, 3}, {HighFive::DataSpace::UNLIMITED, 3});
        HighFive::DataSetCreateProps props;
        props.add(HighFive::Chunking(std::vector<hsize_t>{CHUNK, 3}));
        return file->createDataSet<double>(path, space, props);
    }

    void flush() {
        if (timeBuf.empty()) return;
        const std::size_t n = timeBuf.size();
        const std::size_t start = rowsWritten;

        auto &timeDs = datasets.at("time");
        timeDs.resize({start + n});
        timeDs.select({start}, {n}).write(timeBuf);

        for (auto &[path, buf] : vecBuf) {
            auto &ds = datasets.at(path);
            ds.resize({start + n, 3});
            ds.select({start, 0}, {n, 3}).write(buf);
            buf.clear();
        }
        timeBuf.clear();
        rowsWritten += n;
    }
};

Hdf5IO::Hdf5IO(): fOutFilename("proton.h5"), fImpl(std::make_unique<Impl>()) {

}

Hdf5IO::~Hdf5IO() = default;

Hdf5IO *Hdf5IO::GetInstance() {
    static Hdf5IO instance;
    return &instance;
}

void Hdf5IO::Init() {
    fImpl->file = std::make_unique<HighFive::File>(fOutFilename, HighFive::File::Truncate);
    std::cout << "[Hdf5IO] Writing to " << fOutFilename << std::endl;
}

void Hdf5IO::InitBranches() {
    // time: 1D extensible
    HighFive::DataSpace tspace({0}, {HighFive::DataSpace::UNLIMITED});
    HighFive::DataSetCreateProps tprops;
    tprops.add(HighFive::Chunking(std::vector<hsize_t>{CHUNK}));
    fImpl->datasets.emplace("time", fImpl->file->createDataSet<double>("time", tspace, tprops));

    auto addBody = [&](const std::string &group, const std::string &name) {
        const std::string base = "/" + group + "/" + name;
        fImpl->datasets.emplace(base + "/position", fImpl->makeVecDataset(base + "/position"));
        fImpl->datasets.emplace(base + "/velocity", fImpl->makeVecDataset(base + "/velocity"));
        fImpl->vecBuf[base + "/position"];
        fImpl->vecBuf[base + "/velocity"];
        std::cout << "[Hdf5IO] " << name << " IO Initialised" << std::endl;
    };

    for (Planets *p : RunManager::GetInstance()->GetPlanetList())
        addBody("Planets", p->GetName());
    for (Moons *m : RunManager::GetInstance()->GetMoonList())
        addBody("Moons", m->GetName());
    for (Satellite *s : RunManager::GetInstance()->GetSatelliteList())
        addBody("Satellites", s->GetName());
}

void Hdf5IO::SaveStep(double time) {
    fImpl->timeBuf.push_back(time);
    fImpl->totalSteps++;

    auto push = [&](const std::string &group, const std::string &name,
                    const Vector3D &pos, const Vector3D &vel) {
        const std::string base = "/" + group + "/" + name;
        fImpl->vecBuf[base + "/position"].push_back({pos.GetX(), pos.GetY(), pos.GetZ()});
        fImpl->vecBuf[base + "/velocity"].push_back({vel.GetX(), vel.GetY(), vel.GetZ()});
    };

    for (Planets *p : RunManager::GetInstance()->GetPlanetList())
        push("Planets", p->GetName(), p->GetPosition(), p->GetVelocity());
    for (Moons *m : RunManager::GetInstance()->GetMoonList())
        push("Moons", m->GetName(), m->GetPosition(), m->GetVelocity());
    for (Satellite *s : RunManager::GetInstance()->GetSatelliteList())
        push("Satellites", s->GetName(), s->GetPosition(), s->GetVelocity());

    if (fImpl->timeBuf.size() >= CHUNK)
        fImpl->flush();
}

void Hdf5IO::EndRun() {
    fImpl->flush();
    fImpl->file->flush();
    std::cout << "[Hdf5IO] Wrote " << fImpl->totalSteps << " steps to " << fOutFilename << std::endl;
}
