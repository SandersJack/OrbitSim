#include "Mars.hh"
#include "Vector3D.hh"
#include <iostream>

Mars::Mars() {
    Init();
}

Mars::~Mars(){

}

void Mars::Init(){
    MarsParameters *EParam = MarsParameters::GetInstance();
    fMass = EParam->GetMass();
    fRadius = EParam->GetRadius();
    fName = "Mars";
}

Vector3D Mars::ComputeAcceleration(){
    Vector3D sun(0, 0, 0);
    double sun_mass = 1.989e30;
    return PhysicsEqs::GetInstance()->GetAcceleration(fPosition, sun, fMass, sun_mass, 0.1);
}

void Mars::PrintPos(){
    //std::cout << fPosition->GetX() << " " << fPosition->GetY() << " " << fPosition->GetZ() << std::endl;
}