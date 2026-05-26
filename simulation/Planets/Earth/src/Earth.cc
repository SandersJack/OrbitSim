#include "Earth.hh"
#include "Vector3D.hh"
#include <iostream>

Earth::Earth() {
    Init();
}

Earth::~Earth(){

}

void Earth::Init(){
    EarthParameters *EParam = EarthParameters::GetInstance();
    fMass = EParam->GetMass();
    fRadius = EParam->GetRadius();
    fName = "Earth";
}


void Earth::PrintPos(){
    //std::cout << fPosition->GetX() << " " << fPosition->GetY() << " " << fPosition->GetZ() << std::endl;
}
