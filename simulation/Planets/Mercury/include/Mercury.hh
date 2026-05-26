#ifndef Mercury_H
#define Mercury_H

#include "Planets.hh"
#include "MercuryParameters.hh"
#include "PhysicsEqs.hh"
#include "Vector3D.hh"


class Mercury: public Planets {
    public:
        Mercury();
        ~Mercury();

        void Init();
        void PrintPos();

};

#endif
