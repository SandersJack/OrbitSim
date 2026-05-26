#ifndef Venus_H
#define Venus_H

#include "Planets.hh"
#include "VenusParameters.hh"
#include "PhysicsEqs.hh"
#include "Vector3D.hh"


class Venus: public Planets {
    public:
        Venus();
        ~Venus();

        void Init();
        void PrintPos();

        virtual Vector3D ComputeAcceleration();
};

#endif
