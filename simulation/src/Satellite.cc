#include "Satellite.hh"
#include "PhysicsEqs.hh"
#include <cmath>

Satellite::Satellite():fMass(0), fdt(0) , fTime(0), fName("c"), fStartAngle(-1), fStartTime(-1), fStart(true) {

}

Satellite::~Satellite(){}

void Satellite::SetStartPosition(double body_radius, Vector3D body_val, double sat_val){

    fInclination = 0;
    fLongitude = 180 * M_PI / 180.0;
    fOrbitRadius = body_radius + sat_val * 1e3;
    fPosition = Vector3D();
    fPosition.SetX(fOrbitRadius * cos(fInclination) * cos(fLongitude) + body_val.GetX());
    fPosition.SetY(fOrbitRadius * cos(fInclination) * sin(fLongitude) + body_val.GetY());
    fPosition.SetZ(fOrbitRadius * sin(fInclination) + body_val.GetZ());

    fStepsSinceLaunch = 0;

}

void Satellite::SetStartVelocity(Vector3D body_val, double sat_val){

    fVelocity = Vector3D(body_val);
    fDeltaV = sat_val;
}


void Satellite::NextStep() {

    if (fStartAngle != -1 && fStartTime > -1){
        std::cout << "[Satellite] Error: Can only use Either StartAngle or StartTime" <<std::endl;
        exit(1);
    }

    bool waiting_to_launch =
        (fBody->GetAngle() * 180 / M_PI < fStartAngle && fStart && fStartAngle != -1) ||
        (fTime < fStartTime && fStart && fStartTime != -1);

    if (waiting_to_launch) {
        // Park the satellite on the body's surface (radial line to Sun) until launch.
        Vector3D direction_to_sun = fBody->GetPosition();
        direction_to_sun.Normalize();
        fPosition = fBody->GetPosition() + direction_to_sun * fOrbitRadius;
    } else {
        if (fStart) {
            // Launch: place on surface, inherit body velocity, apply DeltaV along velocity direction.
            Vector3D direction_to_sun = fBody->GetPosition();
            direction_to_sun.Normalize();
            fPosition = fBody->GetPosition() + direction_to_sun * fOrbitRadius;

            fVelocity = Vector3D(fBody->GetVelocity());
            Vector3D direction = fVelocity;
            direction.Normalize();
            fVelocity.Add(direction.Multiply(fDeltaV));
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
