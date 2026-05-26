#include "Mercury.hh"
#include "Vector3D.hh"
#include <iostream>

Mercury::Mercury() {
    Init();
}

Mercury::~Mercury(){

}

void Mercury::Init(){
    MercuryParameters *EParam = MercuryParameters::GetInstance();
    fMass = EParam->GetMass();
    fRadius = EParam->GetRadius();
    fName = "Mercury";
}

Vector3D Mercury::ComputeAcceleration(){
    Vector3D sun(0, 0, 0);
    double sun_mass = 1.989e30;
    return PhysicsEqs::GetInstance()->GetAcceleration(fPosition, sun, fMass, sun_mass, 0.1);
}

void Mercury::PrintPos(){
    //std::cout << fPosition->GetX() << " " << fPosition->GetY() << " " << fPosition->GetZ() << std::endl;
}