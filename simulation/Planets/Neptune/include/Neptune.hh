#ifndef Neptune_H
#define Neptune_H

#include "Planets.hh"
#include "NeptuneParameters.hh"
#include "PhysicsEqs.hh"
#include "Vector3D.hh"


class Neptune: public Planets {
    public:
        Neptune();
        ~Neptune();

        void Init();
        void PrintPos();

        virtual Vector3D ComputeAcceleration();
};

#endif
