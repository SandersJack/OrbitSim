#ifndef Earth_H
#define Earth_H

#include "Planets.hh"
#include "EarthParameters.hh"
#include "PhysicsEqs.hh"
#include "Vector3D.hh"


class Earth: public Planets {
    public:
        Earth();
        ~Earth();

        void Init();
        void PrintPos();

};

#endif
