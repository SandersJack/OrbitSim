#include "Uranus.hh"
#include "Vector3D.hh"
#include <iostream>

Uranus::Uranus() {
    Init();
}

Uranus::~Uranus(){

}

void Uranus::Init(){
    UranusParameters *EParam = UranusParameters::GetInstance();
    fMass = EParam->GetMass();
    fRadius = EParam->GetRadius();
    fName = "Uranus";
}

Vector3D Uranus::ComputeAcceleration(){
    Vector3D sun(0, 0, 0);
    double sun_mass = 1.989e30;
    return PhysicsEqs::GetInstance()->GetAcceleration(fPosition, sun, fMass, sun_mass, 0.1);
}

void Uranus::PrintPos(){
    //std::cout << fPosition->GetX() << " " << fPosition->GetY() << " " << fPosition->GetZ() << std::endl;
}