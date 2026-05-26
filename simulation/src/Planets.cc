#include "Planets.hh"

Planets::Planets():fMass(0), fdt(0) , fTime(0), fRadius(0){

}

Planets::~Planets(){}

void Planets::NextStep() {
    // Velocity-Verlet (kick-drift-kick) in inertial heliocentric coordinates.
    Vector3D a0 = ComputeAcceleration();
    Vector3D v_half = fVelocity + a0 * (fdt * 0.5);
    fPosition = fPosition + v_half * fdt;
    Vector3D a1 = ComputeAcceleration();
    fVelocity = v_half + a1 * (fdt * 0.5);
    fAcceleration = a1;
    fTime += fdt;
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
