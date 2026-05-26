#include "Moon.hh"
#include "Earth.hh"
#include "Vector3D.hh"
#include <iostream>

Moon::Moon() {
    Init();
}

Moon::~Moon(){

}

void Moon::Init(){
    MoonParameters *EParam = MoonParameters::GetInstance();
    fMass = EParam->GetMass();
    fPosition = Vector3D(EParam->GetStartPos());
    fVelocity = Vector3D(EParam->GetStartVel());
    fName = "Moon";
}

Vector3D Moon::ComputeAcceleration(){
    // Moon state stays geocentric until full n-body lockstep lands (#15).
    // The Sun term is the tidal differential, a_sun_on_moon - a_sun_on_earth,
    // computed in the inertial frame from heliocentric positions.
    Vector3D origin(0, 0, 0);
    double sun_mass = 1.989e30;
    PhysicsEqs *physics = PhysicsEqs::GetInstance();

    Vector3D helio_moon = fPosition + fBody->GetPosition();
    Vector3D Sun_on_Moon  = physics->GetAcceleration(helio_moon, origin, fMass, sun_mass, 0.1);
    Vector3D Sun_on_Earth = physics->GetAcceleration(fBody->GetPosition(), origin, fBody->GetMass(), sun_mass, 0.1);
    Vector3D Moon_Earth   = physics->GetAcceleration(fPosition, origin, fMass, fBody->GetMass(), 0.1);

    Vector3D a = Moon_Earth;
    a.AddX(Sun_on_Moon.GetX() - Sun_on_Earth.GetX());
    a.AddY(Sun_on_Moon.GetY() - Sun_on_Earth.GetY());
    a.AddZ(Sun_on_Moon.GetZ() - Sun_on_Earth.GetZ());
    return a;
}

void Moon::PrintPos(){
    //std::cout << fPosition->GetX() << " " << fPosition->GetY() << " " << fPosition->GetZ() << std::endl;
}
