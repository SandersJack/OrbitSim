#ifndef Mars_H
#define Mars_H

#include "Planets.hh"
#include "MarsParameters.hh"
#include "PhysicsEqs.hh"
#include "Vector3D.hh"


class Mars: public Planets {
    public:
        Mars();
        ~Mars();

        void Init();
        void PrintPos();

        virtual Vector3D ComputeAcceleration();
};

#endif
