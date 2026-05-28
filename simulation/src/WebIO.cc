#include "WebIO.hh"
#include "JsonWriter.hh"

#include "Planets.hh"
#include "RunManager.hh"

WebIO::WebIO(): fOutFilename("proton.root") {
    std::cout << "[WEBIO] Saving to JSON file for Web Use" << std::endl;
}

WebIO *WebIO::GetInstance() {
    static WebIO instance;
    return &instance;
}

void WebIO::Init(){
    fJsonWriter = JsonWriter::GetInstance();

    CelestialInfo celestialInfo;

    celestialInfo.planets = RunManager::GetInstance()->GetPlanetList();
    celestialInfo.moons = RunManager::GetInstance()->GetMoonList();
    celestialInfo.satellites = RunManager::GetInstance()->GetSatelliteList();

    fJsonWriter->SetCelestialInfo(celestialInfo);
}

void WebIO::SaveStep(double time, bool lastStep) {
    fJsonWriter->SaveStep(time, lastStep);
}

void WebIO::EndRun() {
    fJsonWriter->EndRun();
}