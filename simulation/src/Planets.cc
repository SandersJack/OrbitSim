#include "Planets.hh"
#include "PhysicsEqs.hh"
#include "RunManager.hh"
#include "Moons.hh"

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

    constexpr double G = 6.6743e-11;
    constexpr double C2 = 8.987551787368176e16; // speed of light squared (m/s)^2

    // Barycentric n-body: Sun is just another entry in fPlanets, no special case.
    for (Planets *other : RunManager::GetInstance()->GetPlanetList()) {
        if (other == this) continue;
        Vector3D d = physics->GetAcceleration(fPosition, other->GetPosition(), fMass, other->GetMass(), 0.1);
        acc.AddX(d.GetX());
        acc.AddY(d.GetY());
        acc.AddZ(d.GetZ());

        // 1PN Schwarzschild correction to Sun's pull (Will 1993, §6.6):
        //   a_GR = (GM/c²r³) [(4GM/r - v²) r + 4(r·v) v]
        // Drives Mercury's 43"/century perihelion precession; negligible for
        // outer planets but harmless to include.
        if (other->GetName() == "Sun") {
            const double rx = fPosition.GetX() - other->GetPosition().GetX();
            const double ry = fPosition.GetY() - other->GetPosition().GetY();
            const double rz = fPosition.GetZ() - other->GetPosition().GetZ();
            const double r2 = rx*rx + ry*ry + rz*rz;
            const double r  = std::sqrt(r2);
            const double vx = fVelocity.GetX() - other->GetVelocity().GetX();
            const double vy = fVelocity.GetY() - other->GetVelocity().GetY();
            const double vz = fVelocity.GetZ() - other->GetVelocity().GetZ();
            const double v2 = vx*vx + vy*vy + vz*vz;
            const double r_dot_v = rx*vx + ry*vy + rz*vz;
            const double GM = G * other->GetMass();
            const double pref = GM / (C2 * r2 * r);
            const double s1 = 4.0*GM/r - v2;
            acc.AddX(pref * (s1 * rx + 4.0 * r_dot_v * vx));
            acc.AddY(pref * (s1 * ry + 4.0 * r_dot_v * vy));
            acc.AddZ(pref * (s1 * rz + 4.0 * r_dot_v * vz));
        }
    }

    // Backreaction from moons: any moon orbiting some planet pulls on every
    // planet too. For Earth feeling its own Moon this is the ~4675 km wobble
    // around the Earth-Moon barycenter.
    for (Moons *m : RunManager::GetInstance()->GetMoonList()) {
        Vector3D moon_abs = m->fBody->GetPosition() + m->GetPosition();
        Vector3D d = physics->GetAcceleration(fPosition, moon_abs, fMass, m->GetMass(), 0.1);
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
