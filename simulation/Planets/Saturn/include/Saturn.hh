#ifndef Saturn_H
#define Saturn_H

#include "Planets.hh"
#include "SaturnParameters.hh"
#include "PhysicsEqs.hh"
#include "Vector3D.hh"


class Saturn: public Planets {
    public:
        Saturn();
        ~Saturn();

        void Init();
        void PrintPos();

        virtual void NextStep();
};

#endif
