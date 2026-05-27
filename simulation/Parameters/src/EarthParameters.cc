#include "EarthParameters.hh"
#include <iostream>

EarthParameters::EarthParameters(){
    Init();
}

EarthParameters::~EarthParameters(){
    
}

EarthParameters *EarthParameters::fInstance = nullptr;

EarthParameters *EarthParameters::GetInstance() {
    if(!fInstance)
        fInstance = new EarthParameters();
    return fInstance;
}

void EarthParameters::Init() {
    fAU = 1.495e11;
    fStartPos = new Vector3D(-0.18244088 * fAU, 0.8851256 * fAU, 0.38391855 * fAU);
    fStartVel = new Vector3D(-29788.29207513, -4954.30121493, -2146.64910384);
    fMass = 5.972168e+24;  // GM/G with GM_earth=3.986004e+14
    fRadius = 6371e3; 
}