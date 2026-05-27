#include "NeptuneParameters.hh"
#include <iostream>
#include <cmath>

NeptuneParameters::NeptuneParameters(){
    Init();
}

NeptuneParameters::~NeptuneParameters(){
    
}

NeptuneParameters *NeptuneParameters::fInstance = nullptr;

NeptuneParameters *NeptuneParameters::GetInstance() {
    if(!fInstance)
        fInstance = new NeptuneParameters();
    return fInstance;
}

void NeptuneParameters::Init() {
    fAU = 1.495e11;
    double r = 29.88712325302;
    double v = 5.47e3;
    double angle = -237.97 * M_PI/180;
    fStartPos = new Vector3D(r*fAU*cos(angle),r*fAU*sin(angle),0);
    fStartVel = new Vector3D(-v*sin(angle),v*cos(angle),0);
    fMass = 1.024306e+26;  // GM/G with GM_neptune=6.836527e+15
    fRadius = 24764e3;
}