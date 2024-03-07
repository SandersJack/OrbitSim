#include "MarsParameters.hh"
#include <iostream>
#include <cmath>

MarsParameters::MarsParameters(){
    Init();
}

MarsParameters::~MarsParameters(){
    
}

MarsParameters *MarsParameters::fInstance = nullptr;

MarsParameters *MarsParameters::GetInstance() {
    if(!fInstance)
        fInstance = new MarsParameters();
    return fInstance;
}

void MarsParameters::Init() {
    fAU = 1.495e11;
    fStartPos = new Vector3D(-0.2941866363887969 * fAU, -1.3255288545044295 * fAU, -0.5999700052291439 * fAU);
    fStartVel = new Vector3D(24693.281204453255, -2140.7850869821505, -1647.7408841891843);
    fMass = 6.39e23;
    fRadius = 3389.5e3;
}