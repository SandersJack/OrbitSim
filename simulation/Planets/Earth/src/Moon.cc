#include "Moon.hh"
#include "Earth.hh"
#include "RunManager.hh"
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
    // Moon state is geocentric (Moon position relative to Earth). The
    // acceleration in the Earth-co-moving frame is:
    //   a_geo_moon = a_helio_moon - a_helio_earth
    //              = direct Earth pull
    //              + sum over every other body i of (pull_on_moon - pull_on_earth)
    // The differential form keeps the strong common-mode Sun term out of the
    // integrator, which would otherwise dwarf the orbital signal.
    Vector3D origin(0, 0, 0);
    PhysicsEqs *physics = PhysicsEqs::GetInstance();

    // Direct Earth pull (Earth sits at origin in the geocentric frame).
    Vector3D a = physics->GetAcceleration(fPosition, origin, fMass, fBody->GetMass(), 0.1);

    // Tidal differential from every other body in fPlanets, Sun included.
    Vector3D helio_moon = fPosition + fBody->GetPosition();
    Vector3D earth_pos  = fBody->GetPosition();
    double earth_mass   = fBody->GetMass();
    for (Planets *b : RunManager::GetInstance()->GetPlanetList()) {
        if (b == fBody) continue;
        Vector3D body_pos = b->GetPosition();
        double body_mass = b->GetMass();
        Vector3D pull_on_moon  = physics->GetAcceleration(helio_moon, body_pos, fMass,       body_mass, 0.1);
        Vector3D pull_on_earth = physics->GetAcceleration(earth_pos,  body_pos, earth_mass,  body_mass, 0.1);
        a.AddX(pull_on_moon.GetX() - pull_on_earth.GetX());
        a.AddY(pull_on_moon.GetY() - pull_on_earth.GetY());
        a.AddZ(pull_on_moon.GetZ() - pull_on_earth.GetZ());
    }
    return a;
}

void Moon::PrintPos(){
    //std::cout << fPosition->GetX() << " " << fPosition->GetY() << " " << fPosition->GetZ() << std::endl;
}
