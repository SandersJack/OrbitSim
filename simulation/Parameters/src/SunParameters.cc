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
    // Tied to the IAU heliocentric gravitational parameter
    //   GM_sun = 1.32712440018e20 m³/s² (best-known to ~1 part in 10^11)
    // divided by our G = 6.6743e-11 so the product matches what JPL
    // ephemerides assume — keeps inner-planet orbits in sync with the
    // initial state vectors we pull from DE421.
    fMass = 1.988416e30;
    fRadius = 6.957e8;
}
