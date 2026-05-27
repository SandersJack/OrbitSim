#ifndef SunParameters_H
#define SunParameters_H

class SunParameters {
    public:
        SunParameters();
        ~SunParameters();

        void Init();

        static SunParameters *GetInstance();

        double GetMass() { return fMass; }
        double GetRadius() { return fRadius; }

    private:
        static SunParameters *fInstance;
        double fMass;
        double fRadius;
};

#endif
