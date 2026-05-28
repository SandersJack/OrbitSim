#include "PhysicsEqs.hh"
#include "RunManager.hh"
#include <cmath>
#include <iostream>

PhysicsEqs::PhysicsEqs(): fG_const(0){
    fG_const = 6.67430e-11;
}

PhysicsEqs::~PhysicsEqs(){
}

PhysicsEqs *PhysicsEqs::GetInstance() {
    static PhysicsEqs instance;
    return &instance;
}


Vector3D PhysicsEqs::GetAcceleration(Vector3D pos1, Vector3D pos2, double mass1, double mass2, double soft){

    double dx = (pos1.GetX() - pos2.GetX());
    double dy = (pos1.GetY() - pos2.GetY());
    double dz = (pos1.GetZ() - pos2.GetZ());

    double inv_r3 = pow((pow(dx,2) + pow(dy,2) + pow(dz,2)),(-1.5));

    Vector3D acc_1 = Vector3D();
    acc_1.SetX(-fG_const * (dx * inv_r3) * mass2);
    acc_1.SetY(-fG_const * (dy * inv_r3) * mass2);
    acc_1.SetZ(-fG_const * (dz * inv_r3) * mass2);

    return acc_1;
}

static double GetSofteningFactor(double distance) {

  double baseSoftening = 1;
  double threshold = 1e8;
  double soft = baseSoftening; //* (threshold / distance);
  //std::cout << "Soft " << soft << " " << distance << std::endl;
  return soft;
}

Vector3D PhysicsEqs::GetSatelliteAcceleration(Vector3D pos1, double mass1, std::string bodyStartName, int stepsSinceLaunch){

    std::vector<Planets*> fPlanets = RunManager::GetInstance()->GetPlanetList();
    Vector3D acc_1 = Vector3D();

    // Sum gravity from every body in the system (Sun is one of them now).
    for(Planets *p : fPlanets){

      double dx_n = (pos1.GetX() - p->GetPosition().GetX());
      double dy_n = (pos1.GetY() - p->GetPosition().GetY());
      double dz_n = (pos1.GetZ() - p->GetPosition().GetZ());

      double d = sqrt(dx_n * dx_n + dy_n * dy_n + dz_n * dz_n);
      double soft = GetSofteningFactor(d);

      double inv_r3 = pow((pow(dx_n,2) + pow(dy_n,2) + pow(dz_n,2) + pow(soft,2)),(-1.5));

      acc_1.AddX(-fG_const * (dx_n * inv_r3) * p->GetMass());
      acc_1.AddY(-fG_const * (dy_n * inv_r3) * p->GetMass());
      acc_1.AddZ(-fG_const * (dz_n * inv_r3) * p->GetMass());

    }

    return acc_1;
}

