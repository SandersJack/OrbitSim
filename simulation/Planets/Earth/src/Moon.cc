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
    // Moon stays geocentric: in heliocentric coords its Sun-driven dv per
    // step is ~500 m/s (much larger than the 1 km/s orbital velocity), and
    // tiny mis-cancellation against Earth's nearly-identical Sun pull
    // distorts the orbit. The tidal formulation works with the small
    // a_sun_on_moon - a_sun_on_earth differential directly.
    Vector3D origin(0, 0, 0);
    const double sun_mass = 1.989e30;
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
