#include "Venus.hh"
#include "Vector3D.hh"
#include <iostream>

Venus::Venus() {
    Init();
}

Venus::~Venus(){

}

void Venus::Init(){
    VenusParameters *EParam = VenusParameters::GetInstance();
    fMass = EParam->GetMass();
    fRadius = EParam->GetRadius();
    fName = "Venus";
}

Vector3D Venus::ComputeAcceleration(){
    Vector3D sun(0, 0, 0);
    double sun_mass = 1.989e30;
    return PhysicsEqs::GetInstance()->GetAcceleration(fPosition, sun, fMass, sun_mass, 0.1);
}

void Venus::PrintPos(){
    //std::cout << fPosition->GetX() << " " << fPosition->GetY() << " " << fPosition->GetZ() << std::endl;
}