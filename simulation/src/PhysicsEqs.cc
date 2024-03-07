#include "PhysicsEqs.hh"
#include "RunManager.hh"
#include <cmath>
#include <iostream>

PhysicsEqs::PhysicsEqs(): fG_const(0){
    fG_const = 6.67430e-11;
}

PhysicsEqs::~PhysicsEqs(){
}

PhysicsEqs *PhysicsEqs::fInstance = nullptr;

PhysicsEqs *PhysicsEqs::GetInstance() {
  if(!fInstance)
    fInstance = new PhysicsEqs();
  return fInstance;
};


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

    /// Calc for SUN

    double dx = (pos1.GetX() - 0);
    double dy = (pos1.GetY() - 0);
    double dz = (pos1.GetZ() - 0);

    double soft = GetSofteningFactor(sqrt(dx * dx + dy * dy + dz * dz));

    double inv_r3 = pow((pow(dx,2) + pow(dy,2) + pow(dz,2) + pow(0.1 ,2)),(-1.5));

    double sun_mass = 1.989e30;
    
    acc_1.AddX(-fG_const * (dx * inv_r3) * sun_mass);
    acc_1.AddY(-fG_const * (dy * inv_r3) * sun_mass);
    acc_1.AddZ(-fG_const * (dz * inv_r3) * sun_mass);


    // Calc for Planets
    for(Planets *p : fPlanets){

      std::string name = p->GetName();

      if(name == bodyStartName && stepsSinceLaunch < 11){
        continue; //Ignore start body for 10 steps after launch
      }
        
      double dx_n = (pos1.GetX() - p->GetPosition().GetX());
      double dy_n = (pos1.GetY() - p->GetPosition().GetY());
      double dz_n = (pos1.GetZ() - p->GetPosition().GetZ());

      double d = sqrt(dx_n * dx_n + dy_n * dy_n + dz_n * dz_n);

      soft = GetSofteningFactor(d);

      double inv_r3 = pow((pow(dx_n,2) + pow(dy_n,2) + pow(dz_n,2) + pow(soft,2)),(-1.5));
      
      acc_1.AddX(-fG_const * (dx * inv_r3) * p->GetMass());
      acc_1.AddY(-fG_const * (dy * inv_r3) * p->GetMass());
      acc_1.AddZ(-fG_const * (dz * inv_r3) * p->GetMass());

    }

    return acc_1;
}

std::pair<Vector3D, Vector3D> PhysicsEqs::RungeKuttaStep(Vector3D position, Vector3D velocity, double mass, double dt, std::string bodyStartName, int stepsSinceLaunch) {
    // Initial acceleration
    Vector3D k1 = GetSatelliteAcceleration(position, mass, bodyStartName, stepsSinceLaunch);

    // Update position and velocity using k1
    Vector3D pos_k2 = position + velocity * (dt / 2.0);
    Vector3D vel_k2 = velocity + k1 * (dt / 2.0);
    
    // Calculate acceleration at pos_k2
    Vector3D k2 = GetSatelliteAcceleration(pos_k2, mass, bodyStartName, stepsSinceLaunch);

    // Update position and velocity using k2
    Vector3D pos_k3 = position + vel_k2 * (dt / 2.0);
    Vector3D vel_k3 = velocity + k2 * (dt / 2.0);
    
    // Calculate acceleration at pos_k3
    Vector3D k3 = GetSatelliteAcceleration(pos_k3, mass, bodyStartName, stepsSinceLaunch);

    // Update position and velocity using k3
    Vector3D pos_k4 = position + vel_k3 * dt;
    Vector3D vel_k4 = velocity + k3 * dt;
    
    // Calculate acceleration at pos_k4
    Vector3D k4 = GetSatelliteAcceleration(pos_k4, mass, bodyStartName, stepsSinceLaunch);

    // Combine results using weighted averages
    Vector3D newPos = position + (velocity + vel_k2 * 2.0 + vel_k3 * 2.0 + vel_k4) * (dt / 6.0);
    Vector3D newVel = velocity + (k1 + k2 * 2.0 + k3 * 2.0 + k4) * (dt / 6.0);

    return std::make_pair(newPos, newVel);
}