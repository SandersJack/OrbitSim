#ifndef Moons_H
#define Moons_H

#include "Planets.hh"
#include "Vector3D.hh"
#include <cmath>
#include <string>

class Moons {

    public:
        Moons();
        virtual ~Moons();

        void SetPosition(Vector3D val){ fPosition = val;};
        void SetVelocity(Vector3D val){ fVelocity = val;};
        void SetAcceleration(Vector3D val){ fAcceleration = val;}
        void SetMass(double val){ fMass = val;};
        void SetName(const std::string &val) { fName = val; }

        Vector3D GetPosition(){ return fPosition;}
        Vector3D GetVelocity(){ return fVelocity;}
        Vector3D GetAcceleration(){ return fAcceleration;}
        double GetMass(){return fMass;}
        std::string GetName() const { return fName; }

        double GetAngle(){return atan(fPosition.GetY()/fPosition.GetX());}

        void SetTimeStep(double val){ fdt = val;}

        void SetBody(Planets *val) {fBody = val;}

        virtual Vector3D ComputeAcceleration() = 0;
        void KickHalf();
        void Drift();
        void RecomputeAcceleration();

    public:

        Planets *fBody;

        Vector3D fPosition;
        Vector3D fVelocity;
        Vector3D fAcceleration;

        double fMass;
        double fdt;
        double fTime;

        std::string fName;
};

#endif
