#ifndef Hdf5IO_H
#define Hdf5IO_H

#include <string>
#include <memory>

class Hdf5IO {

    public:
        Hdf5IO();
        ~Hdf5IO();

        static Hdf5IO *GetInstance();

        void Init();
        void InitBranches();
        void SaveStep(double time);
        void EndRun();

        void SetFilename(const std::string &val) { fOutFilename = val; }

    private:
        std::string fOutFilename;

        // HighFive handles + flush buffers live in the .cc (PIMPL) so the
        // HDF5 headers don't leak into everything that includes RunManager.hh.
        struct Impl;
        std::unique_ptr<Impl> fImpl;
};

#endif
