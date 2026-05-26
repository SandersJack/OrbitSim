#ifndef Uranus_H
#define Uranus_H

#include "Planets.hh"
#include "UranusParameters.hh"
#include "PhysicsEqs.hh"
#include "Vector3D.hh"


class Uranus: public Planets {
    public:
        Uranus();
        ~Uranus();

        void Init();
        void PrintPos();

};

#endif
