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


void Saturn::PrintPos(){
    //std::cout << fPosition->GetX() << " " << fPosition->GetY() << " " << fPosition->GetZ() << std::endl;
}