#include "Jupiter.hh"
#include "Vector3D.hh"
#include <iostream>

Jupiter::Jupiter() {
    Init();
}

Jupiter::~Jupiter(){

}

void Jupiter::Init(){
    JupiterParameters *EParam = JupiterParameters::GetInstance();
    fMass = EParam->GetMass();
    fRadius = EParam->GetRadius();
    fName = "Jupiter";
}

Vector3D Jupiter::ComputeAcceleration(){
    Vector3D sun(0, 0, 0);
    double sun_mass = 1.989e30;
    return PhysicsEqs::GetInstance()->GetAcceleration(fPosition, sun, fMass, sun_mass, 0.1);
}

void Jupiter::PrintPos(){
    //std::cout << fPosition->GetX() << " " << fPosition->GetY() << " " << fPosition->GetZ() << std::endl;
}