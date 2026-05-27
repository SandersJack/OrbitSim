#include "Sun.hh"
#include "Vector3D.hh"

Sun::Sun() {
    Init();
}

Sun::~Sun(){

}

void Sun::Init(){
    SunParameters *SParam = SunParameters::GetInstance();
    fMass = SParam->GetMass();
    fRadius = SParam->GetRadius();
    fName = "Sun";
}
