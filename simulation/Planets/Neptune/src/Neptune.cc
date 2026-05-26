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


void Neptune::PrintPos(){
    //std::cout << fPosition->GetX() << " " << fPosition->GetY() << " " << fPosition->GetZ() << std::endl;
}