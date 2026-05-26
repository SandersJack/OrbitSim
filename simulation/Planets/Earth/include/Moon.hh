#ifndef Moon_H
#define Moon_H

#include "Moons.hh"
#include "Earth.hh"
#include "MoonParameters.hh"
#include "PhysicsEqs.hh"
#include "Vector3D.hh"


class Moon: public Moons {
    public:
        Moon();
        ~Moon();

        void Init();
        void PrintPos();

        virtual Vector3D ComputeAcceleration();
};

#endif
