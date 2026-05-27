#include "Planets.hh"
#include "PhysicsEqs.hh"
#include "RunManager.hh"

Planets::Planets():fMass(0), fdt(0) , fTime(0), fRadius(0){

}

Planets::~Planets(){}

double Planets::GetSphereOfInfluenceRadius() const {
    constexpr double sun_mass = 1.989e30;
    const double r = std::sqrt(fPosition.fx * fPosition.fx +
                               fPosition.fy * fPosition.fy +
                               fPosition.fz * fPosition.fz);
    return r * std::pow(fMass / sun_mass, 0.4);
}

double Planets::GetAngle(){

    double angle = atan(fPosition.GetY()/fPosition.GetX());
    if (angle >= 0 && fPosition.GetX() > 0 && fPosition.GetY() > 0){
        return angle;
    } else if (fPosition.GetX() > 0 && fPosition.GetY() < 0){
        return 2*M_PI + angle;
    }
    else{
        return angle +  M_PI;
    }

}

Vector3D Planets::ComputeAcceleration() {
    PhysicsEqs *physics = PhysicsEqs::GetInstance();
    Vector3D acc;

    // Barycentric n-body: Sun is just another entry in fPlanets, no special case.
    for (Planets *other : RunManager::GetInstance()->GetPlanetList()) {
        if (other == this) continue;
        Vector3D d = physics->GetAcceleration(fPosition, other->GetPosition(), fMass, other->GetMass(), 0.1);
        acc.AddX(d.GetX());
        acc.AddY(d.GetY());
        acc.AddZ(d.GetZ());
    }
    return acc;
}

void Planets::KickHalf() {
    fVelocity = fVelocity + fAcceleration * (fdt * 0.5);
}

void Planets::Drift() {
    fPosition = fPosition + fVelocity * fdt;
}

void Planets::RecomputeAcceleration() {
    fAcceleration = ComputeAcceleration();
}
