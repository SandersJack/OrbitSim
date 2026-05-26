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


void Mars::PrintPos(){
    //std::cout << fPosition->GetX() << " " << fPosition->GetY() << " " << fPosition->GetZ() << std::endl;
}