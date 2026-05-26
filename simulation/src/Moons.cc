#include "Moons.hh"

Moons::Moons():fMass(0), fdt(0) , fTime(0){

}

Moons::~Moons(){}

void Moons::NextStep() {
    // Velocity-Verlet, same as Planets. Moons live in inertial heliocentric
    // coordinates after RunManager lifts them from their geocentric initial
    // parameters.
    Vector3D a0 = ComputeAcceleration();
    Vector3D v_half = fVelocity + a0 * (fdt * 0.5);
    fPosition = fPosition + v_half * fdt;
    Vector3D a1 = ComputeAcceleration();
    fVelocity = v_half + a1 * (fdt * 0.5);
    fAcceleration = a1;
    fTime += fdt;
}
