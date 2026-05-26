#include "Satellite.hh"
#include "PhysicsEqs.hh"
#include <cmath>

Satellite::Satellite():fMass(0), fdt(0) , fTime(0), fName("c"), fStartAngle(-1), fStartTime(-1), fStart(true) {

}

Satellite::~Satellite(){}

void Satellite::SetStartPosition(double /*body_radius*/, Vector3D /*body_val*/, double sat_val){
    // Legacy OrbitHeight is retained in fOrbitRadius for any callers that
    // want the LEO-altitude semantics, but launch is now patched-conic so the
    // actual release distance is the body's sphere of influence.
    fOrbitRadius = sat_val * 1e3;
    fStepsSinceLaunch = 0;
}

void Satellite::SetStartVelocity(Vector3D /*body_val*/, double sat_val){
    // sat_val is interpreted as v_infinity (hyperbolic excess) in m/s.
    fDeltaV = sat_val;
}

static Vector3D ProgradeUnit(const Vector3D &v) {
    Vector3D u = v;
    u.Normalize();
    return u;
}

void Satellite::NextStep() {

    if (fStartAngle != -1 && fStartTime > -1){
        std::cout << "[Satellite] Error: Can only use Either StartAngle or StartTime" <<std::endl;
        exit(1);
    }

    // Patched-conic release: launch position sits at the body's sphere of
    // influence, in the body's prograde direction. At that distance the body's
    // gravity is balanced by the Sun's, so the full n-body integrator picks
    // the trajectory up cleanly with no per-step exclusion.
    const double r_soi = fBody->GetSphereOfInfluenceRadius();
    Vector3D prograde = ProgradeUnit(fBody->GetVelocity());
    Vector3D parked = fBody->GetPosition() + prograde * r_soi;

    bool waiting_to_launch =
        (fBody->GetAngle() * 180 / M_PI < fStartAngle && fStart && fStartAngle != -1) ||
        (fTime < fStartTime && fStart && fStartTime != -1);

    if (waiting_to_launch) {
        // Track the body's SOI in lockstep until the launch trigger fires.
        fPosition = parked;
        fVelocity = fBody->GetVelocity();
    } else {
        if (fStart) {
            // Release with v_infinity along the body's prograde direction.
            fPosition = parked;
            fVelocity = fBody->GetVelocity() + prograde * fDeltaV;
            fStart = false;
        }

        // Velocity-Verlet step, matching the planet/moon integrator.
        PhysicsEqs *physics = PhysicsEqs::GetInstance();
        const std::string &launch_body = fBody->GetName();

        Vector3D a0 = physics->GetSatelliteAcceleration(fPosition, fMass, launch_body, fStepsSinceLaunch);
        Vector3D v_half = fVelocity + a0 * (fdt * 0.5);
        fPosition = fPosition + v_half * fdt;
        Vector3D a1 = physics->GetSatelliteAcceleration(fPosition, fMass, launch_body, fStepsSinceLaunch + 1);
        fVelocity = v_half + a1 * (fdt * 0.5);
        fAcceleration = a1;

        fStepsSinceLaunch++;
    }
    fTime += fdt;
}
