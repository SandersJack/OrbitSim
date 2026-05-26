#include "Earth.hh"
#include "Vector3D.hh"
#include <iostream>

Earth::Earth() {
    Init();
}

Earth::~Earth(){

}

void Earth::Init(){
    EarthParameters *EParam = EarthParameters::GetInstance();
    fMass = EParam->GetMass();
    fRadius = EParam->GetRadius();
    fName = "Earth";
}

Vector3D Earth::ComputeAcceleration(){
    Vector3D sun(0, 0, 0);
    double sun_mass = 1.989e30;
    return PhysicsEqs::GetInstance()->GetAcceleration(fPosition, sun, fMass, sun_mass, 0.1);
}

void Earth::PrintPos(){
    //std::cout << fPosition->GetX() << " " << fPosition->GetY() << " " << fPosition->GetZ() << std::endl;
}
