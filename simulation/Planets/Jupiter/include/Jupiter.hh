#ifndef Jupiter_H
#define Jupiter_H

#include "Planets.hh"
#include "JupiterParameters.hh"
#include "PhysicsEqs.hh"
#include "Vector3D.hh"


class Jupiter: public Planets {
    public:
        Jupiter();
        ~Jupiter();

        void Init();
        void PrintPos();

        virtual void NextStep();
};

#endif
