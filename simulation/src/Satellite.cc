#include "Satellite.hh"
#include "PhysicsEqs.hh"
#include <cmath>

ClassImp(Satellite)

Satellite::Satellite():fMass(0), fdt(0) , fTime(0), fName("c"), fStartAngle(-1), fStartTime(-1), fStart(true) {

}

Satellite::~Satellite(){}

void Satellite::SetStartPosition(double body_radius, Vector3D body_val, double sat_val){

    fInclination = 0;
    fLongitude = 180;
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

    if((fBody->GetAngle() *180 / M_PI < fStartAngle && fStart && fStartAngle != -1) || (fTime < fStartTime && fStart && fStartTime != -1)){

        Vector3D direction_to_earth = fBody->GetPosition();
        direction_to_earth.Normalize();

        Vector3D satellite_position = fBody->GetPosition() + direction_to_earth * (fOrbitRadius);

        fPosition = satellite_position;

        /*
        fPosition.SetX(fOrbitRadius * cos(fInclination) * cos(fLongitude) + fBody->GetPosition().GetX());
        fPosition.SetY(fOrbitRadius * cos(fInclination) * sin(fLongitude) + fBody->GetPosition().GetY());
        fPosition.SetZ(fOrbitRadius * sin(fInclination) + fBody->GetPosition().GetZ());
        */
    } else {
        if(fStart){

            Vector3D direction_to_earth = fBody->GetPosition();
            direction_to_earth.Normalize();

            Vector3D satellite_position = fBody->GetPosition() + direction_to_earth * (fOrbitRadius);

            fPosition = satellite_position;

            /*
            fPosition.SetX(fOrbitRadius * cos(fInclination) * cos(fLongitude) + fBody->GetPosition().GetX());
            fPosition.SetY(fOrbitRadius * cos(fInclination) * sin(fLongitude) + fBody->GetPosition().GetY());
            fPosition.SetZ(fOrbitRadius * sin(fInclination) + fBody->GetPosition().GetZ());
            */
            fVelocity = Vector3D(fBody->GetVelocity());

            Vector3D direction = fVelocity;
            direction.Normalize();

            // Scale the velocity vector
            fVelocity.Add(direction.Multiply(fDeltaV));
            fStart=false;
        }

        PhysicsEqs *PhysicsEqs = PhysicsEqs::GetInstance();

        std::pair<Vector3D, Vector3D> out = PhysicsEqs->RungeKuttaStep(fPosition, fVelocity, fMass, fdt, fBody->GetName(), fStepsSinceLaunch);

        fPosition = out.first;
        fVelocity = out.second;

        /*
        Vector3D Sat_a = PhysicsEqs->GetSatelliteAcceleration(fPosition, fMass);

        fAcceleration = Sat_a;

        fVelocity.AddX(Sat_a.GetX()*fdt);
        fVelocity.AddY(Sat_a.GetY()*fdt);
        fVelocity.AddZ(Sat_a.GetZ()*fdt);

        fPosition.AddX(fVelocity.GetX()*fdt);
        fPosition.AddY(fVelocity.GetY()*fdt);
        fPosition.AddZ(fVelocity.GetZ()*fdt);
        */

        fStepsSinceLaunch++; 
    }
    fTime += fdt;
}