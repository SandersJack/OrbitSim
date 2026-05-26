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


void Mercury::PrintPos(){
    //std::cout << fPosition->GetX() << " " << fPosition->GetY() << " " << fPosition->GetZ() << std::endl;
}