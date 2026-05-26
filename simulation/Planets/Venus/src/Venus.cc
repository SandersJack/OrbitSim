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


void Venus::PrintPos(){
    //std::cout << fPosition->GetX() << " " << fPosition->GetY() << " " << fPosition->GetZ() << std::endl;
}