#include "Moons.hh"

Moons::Moons():fMass(0), fdt(0) , fTime(0){

}

Moons::~Moons(){}

void Moons::Kick(double c) {
    fVelocity = fVelocity + fAcceleration * (fdt * c);
}

void Moons::Drift(double c) {
    fPosition = fPosition + fVelocity * (fdt * c);
}

void Moons::RecomputeAcceleration() {
    fAcceleration = ComputeAcceleration();
}
