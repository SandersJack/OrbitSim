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


void Jupiter::PrintPos(){
    //std::cout << fPosition->GetX() << " " << fPosition->GetY() << " " << fPosition->GetZ() << std::endl;
}