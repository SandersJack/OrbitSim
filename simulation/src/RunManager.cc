#include "RunManager.hh"

#include "Earth.hh"
#include "Mars.hh"
#include "Moon.hh"

#include "namedBodies.hh"
#include "InputManager.hh"

#include "Vector3D.hh"
#include "PhysicsEqs.hh"

#include <iostream>
#include <string>
#include <cmath>


RunManager::RunManager(): fdt(0), fStopTime(0) {
}

RunManager *RunManager::fInstance = nullptr;

RunManager *RunManager::GetInstance() {
  if(!fInstance)
    fInstance = new RunManager();
  return fInstance;
};

void RunManager::Init() {
  constexpr double AU = 1.49597870700e11; // IAU 2012 exact

  fPlanetList = InputManager::GetInstance()->GetPlanetList();
  fPlanetAtrMap = InputManager::GetInstance()->GetPlanetAtributesList();

  fMoonMap = InputManager::GetInstance()->GetMoonList();

  fSatMap = InputManager::GetInstance()->GetSatelliteList();
  fSatAtrMap = InputManager::GetInstance()->GetSatelliteAtributesList();

  namedBodies *conv = namedBodies::GetInstance();
  // Init Bodies and Satellites
  std::cout << "[RunManager] Simulation using Planets: " << std::endl;
  for(std::string plan : fPlanetList){
    std::cout << "[RunManager] " << plan << std::endl;
    std::map<std::string, std::vector<double>>::iterator atr_iter = fPlanetAtrMap.find(plan);
    Planets *b = conv->GetPlanetFunc(plan);
    if (atr_iter != fPlanetAtrMap.end() )
    {
      Vector3D pos = Vector3D(atr_iter->second[0] * AU, atr_iter->second[1] * AU, atr_iter->second[2] * AU);
      Vector3D vel = Vector3D(atr_iter->second[3], atr_iter->second[4], atr_iter->second[5]);
      b->SetPosition(pos);
      b->SetVelocity(vel);
    }
    fPlanets.push_back(b);

    std::map<std::string, std::string>::iterator iter = fMoonMap.find(plan);
    if (iter != fMoonMap.end() )
    {
      std::cout << "[RunManager] - " << iter->second << std::endl;
      Moons *m = conv->GetMoonsFunc(iter->second);
      m->SetBody(b);
      fMoons.push_back(m);
    }

    std::map<std::string, std::vector<std::pair<std::string, std::string>>>::iterator sat_iter = fSatMap.find(plan);

    if (sat_iter != fSatMap.end() )
    {
      for(auto sat : sat_iter->second){
        std::cout << "[RunManager] - " << sat.second << std::endl;
        std::map<std::string, std::vector<double>>::iterator atr_iter = fSatAtrMap.find(sat.second);
        Satellite *s = conv->GetSatelliteFunc(sat.first);
        if (atr_iter != fSatAtrMap.end() )
        {
          s->SetBody(b);
          s->SetMass(atr_iter->second[0]);
          s->SetStartPosition(b->GetRadius(), b->GetPosition(), atr_iter->second[1]);
          s->SetStartVelocity(b->GetVelocity(),atr_iter->second[2]);
          s->SetStartAngle(atr_iter->second[3]);
          s->SetStartTime(atr_iter->second[4]);
          s->SetName(sat.second);
        }
        fSatellites.push_back(s);
      }
    }
  }


  for(Planets *p : fPlanets){
    p->SetTimeStep(fdt);
  }

  for(Moons *m : fMoons){
    m->SetTimeStep(fdt);
  }

  for(Satellite *s : fSatellites){
    s->SetTimeStep(fdt);
  }

  if(fWebOut){
    fWebIOManager = WebIO::GetInstance();
    fWebIOManager->Init();
  } else {
    fIOManager = Hdf5IO::GetInstance();
    fIOManager->Init();
    fIOManager->InitBranches();
  }


  fHelper = new OrbitHelper();

  // Seed accelerations at t=0 so the first KickHalf uses real values.
  for (Planets *p : fPlanets) p->RecomputeAcceleration();
  for (Moons *m : fMoons) m->RecomputeAcceleration();
}

void RunManager::Run() {
  PhysicsEqs *PhyscisEq = PhysicsEqs::GetInstance();

  double t = 0;

  // Step the Simualtion by Dt — lockstep velocity-Verlet (KDK).
  for (int i=0; i*fdt < fStopTime*24*60*60; i++){
    // First half-kick: v += a * dt/2 using last step's stored acceleration.
    for(Planets *p : fPlanets) p->KickHalf();
    for(Moons *m : fMoons) m->KickHalf();

    // Drift: r += v * dt.
    for(Planets *p : fPlanets) p->Drift();
    for(Moons *m : fMoons) m->Drift();

    // Recompute accelerations at the new positions, with every body
    // already at t+dt so mutual-gravity terms are self-consistent.
    for(Planets *p : fPlanets) p->RecomputeAcceleration();
    for(Moons *m : fMoons) m->RecomputeAcceleration();

    // Second half-kick: v += a * dt/2 with the fresh acceleration.
    for(Planets *p : fPlanets) p->KickHalf();
    for(Moons *m : fMoons) m->KickHalf();

    // Satellites integrate against the now-finalised planet state.
    for(Satellite *s : fSatellites){
      s->NextStep();
    }

    t += fdt;

    int fDisplayPeriod = 100;
    if(i % fDisplayPeriod == 0) {
    std::pair<double, double> memoryUsage = fHelper->GetMemoryUsage();
    std::cout << "[" << fHelper->TimeString() << "] Running Day "
              << double(i*fdt)/(24*60*60)
              << " [Memory usage -> Virtual: " << memoryUsage.first
              << " MB, Resident: " << memoryUsage.second << " MB]" << std::endl;
    }

    if(fWebOut){
      bool last = false;
      if ((i + 1) * fdt >= fStopTime * 24 * 60 * 60) {
        // Is last one
        last = true;
      }
      fWebIOManager->SaveStep(t, last);
    } else {
      fIOManager->SaveStep(t);
    }

  }
  
  if(fWebOut){
    fWebIOManager->EndRun();
  } else {
    fIOManager->EndRun();
  }
  
  std::cout << "[RunManager] Simulation Complete :)" << std::endl;
}

