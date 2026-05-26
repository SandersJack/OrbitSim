#include "Moons.hh"

Moons::Moons():fMass(0), fdt(0) , fTime(0){

}

Moons::~Moons(){}

void Moons::KickHalf() {
    fVelocity = fVelocity + fAcceleration * (fdt * 0.5);
}

void Moons::Drift() {
    fPosition = fPosition + fVelocity * fdt;
}

void Moons::RecomputeAcceleration() {
    fAcceleration = ComputeAcceleration();
}
