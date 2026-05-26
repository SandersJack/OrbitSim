#ifndef Planets_H
#define Planets_H

#include "Vector3D.hh"
#include <cmath>
#include <iostream>
#include <string>

class Planets {

    public:
        Planets();
        virtual ~Planets();

        void SetPosition(Vector3D val){ fPosition = val;};
        void SetVelocity(Vector3D val){ fVelocity = val;};
        void SetAcceleration(Vector3D val){ fAcceleration = val;}
        void SetMass(double val){ fMass = val;};
        void SetName(const std::string &val) { fName = val; }

        Vector3D GetPosition(){ return fPosition;}
        Vector3D GetVelocity(){ return fVelocity;}
        Vector3D GetAcceleration(){ return fAcceleration;}
        double GetMass(){return fMass;}
        double GetRadius() {return fRadius;}
        std::string GetName() const { return fName; }

        double GetAngle();

        void SetTimeStep(double val){ fdt = val;}

        // Lockstep velocity-Verlet phases. RunManager calls these across all
        // bodies so accelerations remain self-consistent under mutual gravity.
        virtual Vector3D ComputeAcceleration();
        void KickHalf();
        void Drift();
        void RecomputeAcceleration();

    public:
        Vector3D fPosition;
        Vector3D fVelocity;
        Vector3D fAcceleration;

        double fMass;
        double fdt;
        double fTime;

        double fRadius;

        std::string fName;
};

#endif
