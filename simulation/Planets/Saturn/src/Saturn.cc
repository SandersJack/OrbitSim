#include "Saturn.hh"
#include "Vector3D.hh"
#include <iostream>

Saturn::Saturn() {
    Init();
}

Saturn::~Saturn(){

}

void Saturn::Init(){
    SaturnParameters *EParam = SaturnParameters::GetInstance();
    fMass = EParam->GetMass();
    fRadius = EParam->GetRadius();
    fName = "Saturn";
}

Vector3D Saturn::ComputeAcceleration(){
    Vector3D sun(0, 0, 0);
    double sun_mass = 1.989e30;
    return PhysicsEqs::GetInstance()->GetAcceleration(fPosition, sun, fMass, sun_mass, 0.1);
}

void Saturn::PrintPos(){
    //std::cout << fPosition->GetX() << " " << fPosition->GetY() << " " << fPosition->GetZ() << std::endl;
}