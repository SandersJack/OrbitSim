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


  // Minor bodies (asteroids, dwarf planets, etc.) — config-driven, no
  // per-body class. Each row becomes a generic Planets instance pushed into
  // fPlanets so it participates in the n-body integration like everything
  // else.
  const double AU_m = AU;
  for (const auto &body : InputManager::GetInstance()->GetAsteroids()) {
    Planets *p = new Planets();
    p->SetName(body.name);
    p->SetMass(body.mass);
    p->SetRadius(body.radius);
    p->SetPosition(Vector3D(body.pos[0] * AU_m, body.pos[1] * AU_m, body.pos[2] * AU_m));
    p->SetVelocity(Vector3D(body.vel[0],        body.vel[1],        body.vel[2]));
    fPlanets.push_back(p);
    std::cout << "[RunManager] minor body: " << body.name << std::endl;
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

  // Yoshida 4th-order symplectic integrator (Forest-Ruth 1990). Three
  // kicks per step (with the middle one negative) and four drifts;
  // truncation error drops from O(dt²) (Verlet) to O(dt⁴), which kills
  // most of the spurious Mercury perihelion precession seen in #14/#30.
  //
  // θ = 1/(2 - 2^(1/3));  drift weights θ/2, (1-θ)/2, (1-θ)/2, θ/2;
  // kick weights θ, 1-2θ, θ.
  constexpr double THETA = 1.3512071919596578;          // 1/(2 - 2^(1/3))
  constexpr double C14   = THETA / 2.0;                 // 0.67560...
  constexpr double C23   = (1.0 - THETA) / 2.0;         // -0.17560...
  constexpr double D13   = THETA;                       // 1.35120...
  constexpr double D2    = 1.0 - 2.0 * THETA;           // -1.70241...

  auto drift = [&](double c) {
      for (Planets *p : fPlanets) p->Drift(c);
      for (Moons *m : fMoons) m->Drift(c);
  };
  auto kick = [&](double c) {
      for (Planets *p : fPlanets) p->Kick(c);
      for (Moons *m : fMoons) m->Kick(c);
  };
  auto recompute = [&]() {
      for (Planets *p : fPlanets) p->RecomputeAcceleration();
      for (Moons *m : fMoons) m->RecomputeAcceleration();
  };

  for (int i=0; i*fdt < fStopTime*24*60*60; i++){
    drift(C14);
    recompute(); kick(D13);
    drift(C23);
    recompute(); kick(D2);
    drift(C23);
    recompute(); kick(D13);
    drift(C14);

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

