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

void Moon::NextStep(){

    PhysicsEqs *PhysicsEqs = PhysicsEqs::GetInstance();

    Vector3D *cen = new Vector3D(0,0,0);
    double sun_mass = 1.989e30;

    // Moon state is geocentric; the Sun term in this frame is the tidal
    // differential, a_sun_on_moon − a_sun_on_earth. Adding the raw inertial
    // Sun pull would launch the Moon out of the system.
    Vector3D helio_pos = fPosition + fBody->GetPosition();
    Vector3D Sun_on_Moon  = PhysicsEqs->GetAcceleration(helio_pos, cen, fMass, sun_mass, 0.1);
    Vector3D Sun_on_Earth = PhysicsEqs->GetAcceleration(fBody->GetPosition(), cen, fBody->GetMass(), sun_mass, 0.1);
    Vector3D Moon_Earth   = PhysicsEqs->GetAcceleration(fPosition, cen, fMass, fBody->GetMass(), 0.1);

    fAcceleration = Moon_Earth;
    fAcceleration.AddX(Sun_on_Moon.GetX() - Sun_on_Earth.GetX());
    fAcceleration.AddY(Sun_on_Moon.GetY() - Sun_on_Earth.GetY());
    fAcceleration.AddZ(Sun_on_Moon.GetZ() - Sun_on_Earth.GetZ());

    fVelocity.AddX(fAcceleration.GetX()*fdt);
    fVelocity.AddY(fAcceleration.GetY()*fdt);
    fVelocity.AddZ(fAcceleration.GetZ()*fdt);

    fPosition.AddX(fVelocity.GetX()*fdt);
    fPosition.AddY(fVelocity.GetY()*fdt);
    fPosition.AddZ(fVelocity.GetZ()*fdt);

    /// This is extremely cheaty but its late and I cant do maths at this time to fix it :)
    fSunPosition.SetX(fPosition.GetX() + fBody->GetPosition().GetX());
    fSunPosition.SetY(fPosition.GetY() + fBody->GetPosition().GetY());
    fSunPosition.SetZ(fPosition.GetZ() + fBody->GetPosition().GetZ());
    
}

void Moon::PrintPos(){
    //std::cout << fPosition->GetX() << " " << fPosition->GetY() << " " << fPosition->GetZ() << std::endl;
}