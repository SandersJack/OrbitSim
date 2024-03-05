#include "WebIO.hh"
#include "JsonWritter.hh"

#include "Planets.hh"
#include "RunManager.hh"

WebIO::WebIO(): fOutFilename("proton.root") {

}

WebIO *WebIO::fInstance = nullptr;

WebIO *WebIO::GetInstance() {
  if(!fInstance)
    fInstance = new WebIO();
  return fInstance;
};

void WebIO::Init(){
    fJsonWritter = JsonWritter::GetInstance();

    CelestialInfo celestialInfo;

    celestialInfo.planets = RunManager::GetInstance()->GetPlanetList();
    celestialInfo.moons = RunManager::GetInstance()->GetMoonList();
    celestialInfo.satellites = RunManager::GetInstance()->GetSatelliteList();

    fJsonWritter->SetCelestialInfo(celestialInfo);
}

void WebIO::SaveStep(double time, bool lastStep) {
    fJsonWritter->SaveStep(time, lastStep);
}

void WebIO::EndRun() {
    fJsonWritter->EndRun();
}