#include "Neptune.hh"
#include "Vector3D.hh"
#include <iostream>

Neptune::Neptune() {
    Init();
}

Neptune::~Neptune(){

}

void Neptune::Init(){
    NeptuneParameters *EParam = NeptuneParameters::GetInstance();
    fMass = EParam->GetMass();
    fRadius = EParam->GetRadius();
    fName = "Neptune";
}

Vector3D Neptune::ComputeAcceleration(){
    Vector3D sun(0, 0, 0);
    double sun_mass = 1.989e30;
    return PhysicsEqs::GetInstance()->GetAcceleration(fPosition, sun, fMass, sun_mass, 0.1);
}

void Neptune::PrintPos(){
    //std::cout << fPosition->GetX() << " " << fPosition->GetY() << " " << fPosition->GetZ() << std::endl;
}