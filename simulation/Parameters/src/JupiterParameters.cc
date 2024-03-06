#include "JupiterParameters.hh"
#include <iostream>
#include <cmath>

JupiterParameters::JupiterParameters(){
    Init();
}

JupiterParameters::~JupiterParameters(){
    
}

JupiterParameters *JupiterParameters::fInstance = nullptr;

JupiterParameters *JupiterParameters::GetInstance() {
    if(!fInstance)
        fInstance = new JupiterParameters();
    return fInstance;
}

void JupiterParameters::Init() {
    fAU = 1.495e11;
    //double r = 4.95057179982;
    //double v = 13.72e3;
    //double angle = -268.187 * M_PI/180;
    fStartPos = new Vector3D(3.48246223235037 * fAU, 3.2984718191970486 * fAU, 1.3290673407721432 * fAU);
    fStartVel = new Vector3D(-9478.338380621848, 8888.42379431389, 4040.5961921722296);
    fMass = 1898.13e24;
    fRadius = 71492e3;
}