#ifndef Sun_H
#define Sun_H

#include "Planets.hh"
#include "SunParameters.hh"
#include "PhysicsEqs.hh"
#include "Vector3D.hh"


class Sun: public Planets {
    public:
        Sun();
        ~Sun();

        void Init();
};

#endif
