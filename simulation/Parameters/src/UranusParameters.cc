#include "UranusParameters.hh"
#include <iostream>
#include <cmath>

UranusParameters::UranusParameters(){
    Init();
}

UranusParameters::~UranusParameters(){
    
}

UranusParameters *UranusParameters::fInstance = nullptr;

UranusParameters *UranusParameters::GetInstance() {
    if(!fInstance)
        fInstance = new UranusParameters();
    return fInstance;
}

void UranusParameters::Init() {
    fAU = 1.495e11;
    double r = 18.26694449067;
    double v = 7.13e3;
    double angle = -112.24 * M_PI/180;
    fStartPos = new Vector3D(r*fAU*cos(angle),r*fAU*sin(angle),0);
    fStartVel = new Vector3D(-v*sin(angle),v*cos(angle),0);
    fMass = 8.681894e+25;  // GM/G with GM_uranus=5.794556e+15
    fRadius = 25559e3;
}