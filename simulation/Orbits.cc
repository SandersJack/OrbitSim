#include "RunManager.hh"
#include "InputManager.hh"

#include <iostream>
#include <string>


int main(int argc, char* argv[]){
    // Default Parameters
    std::string infile = "config/PlanetsList.dat";
    std::string Mooninfile = "config/MoonsList.dat";
    std::string Satinfile = "config/SatelliteList.dat";
    double timeStep = 86400; //seconds
    double stopTime = 1000; //days

    bool web = false;

    // Parse inputs
    for (int i = 1; i < argc; i++)
    {  
        if (i + 1 != argc)
        {
            if (strcmp(argv[i], "-Ts") == 0) // TimeStep
            {                 
                timeStep = std::stod(argv[i + 1]);
                i++;
            } 
            if (strcmp(argv[i], "-St") == 0) // StopTime
            {                 
                stopTime = std::stod(argv[i + 1]);
                i++;
            }
            if (strcmp(argv[i], "-SatList") == 0) // StopTime
            {                 
                Satinfile = (argv[i + 1]);
                i++;
            }
        }
        if (strcmp(argv[i], "-web") == 0) // webOut
            {                 
                web = true;
                continue;
            }
        if (strcmp(argv[i], "-h") == 0) // Help
            {                 
                std::cout << "-----------------------------------" << std::endl;
                std::cout << "Displaying Help Message:" << std::endl;
                std::cout << "-Ts <double> : Time step in seconds" << std::endl;
                std::cout << "-St <double> : Total time in days" << std::endl;
                std::cout << "-web : Outputs data into web freindly format" << std::endl;
                std::cout << "-----------------------------------" << std::endl;
                exit(0);
        }
    }

    std::cout << "##########################################################" << std::endl;
    std::cout << "##    Running Orbit Simulation using Parameters: " << "       ##" << std::endl;
    std::cout << "##    Time Step = " << timeStep/(24*60*60) << " days. Total Time = " << stopTime << " days."  << "       ##" << std::endl;
    std::cout << "##########################################################" << std::endl;

    // Parse config input files
    InputManager::GetInstance()->ParsePlanetFile(infile);
    InputManager::GetInstance()->ParsePlanetFile(Mooninfile);
    InputManager::GetInstance()->ParsePlanetFile(Satinfile);
    RunManager *runMan = RunManager::GetInstance();
    runMan->SetWebOut(web);
    runMan->SetTimeStep(timeStep);
    runMan->SetStopTime(stopTime);
    runMan->Init();
    // Run the Sim
    runMan->Run();
    return 0;
}