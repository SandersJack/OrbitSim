#include "SunParameters.hh"

SunParameters::SunParameters() {
    Init();
}

SunParameters::~SunParameters() {

}

SunParameters *SunParameters::fInstance = nullptr;

SunParameters *SunParameters::GetInstance() {
    if (!fInstance)
        fInstance = new SunParameters();
    return fInstance;
}

void SunParameters::Init() {
    fMass = 1.98892e30; // NASA fact sheet
    fRadius = 6.957e8;
}
