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


void Uranus::PrintPos(){
    //std::cout << fPosition->GetX() << " " << fPosition->GetY() << " " << fPosition->GetZ() << std::endl;
}