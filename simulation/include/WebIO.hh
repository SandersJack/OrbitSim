#ifndef WebIO_H
#define WebIO_H 1 

#include "JsonWriter.hh"

#include <iostream>
#include <string>

class WebIO {
    
    public:
        WebIO();

        static WebIO *GetInstance();

        void Init();
        void SaveStep(double time, bool lastStep);
        void EndRun();

        void SetFilename(const char *val) {fOutFilename = val;}

    private:
        static WebIO *fInstance;

        const char *fOutFilename;

        JsonWriter *fJsonWriter;
};

#endif