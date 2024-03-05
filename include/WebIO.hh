#ifndef WebIO_H
#define WebIO_H 1 

#include "JsonWritter.hh"

#include <iostream>
#include <string>

class WebIO {
    
    public:
        WebIO();

        static WebIO *GetInstance();

        void Init();
        void SaveStep(bool lastStep);
        void EndRun();

        void SetFilename(const char *val) {fOutFilename = val;}

    private:
        static WebIO *fInstance;

        const char *fOutFilename;

        JsonWritter *fJsonWritter;
};

#endif