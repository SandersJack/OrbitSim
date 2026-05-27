#include "InputManager.hh"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

InputManager::InputManager() {

}

InputManager *InputManager::fInstance = nullptr;

InputManager *InputManager::GetInstance() {
    if(!fInstance)
        fInstance = new InputManager();
    return fInstance;
}


void InputManager::ParsePlanetFile(std::string file) {
    std::ifstream inFile;
    std::cout << "[InputManager] Opening Config File: "<< file << std::endl;
    inFile.open(file);

    if (!inFile) {
        std::cout << "Unable to open file" << std::endl;
        exit(1); // terminate with error
    }
    
    std::string x = "c";
    if(file.find("Planets") != std::string::npos) {
        while (std::getline(inFile, x)) {
            if(x.find('#') != std::string::npos)
                continue;

            std::vector<std::string> tokens;
        
            std::istringstream iss(x);
            std::string s;

            while (std::getline(iss, s, ' ')) {
                tokens.push_back(s);
            }
            
            std::string Sat = "";
            std::string name = "";
            std::string Planet = "";
            std::vector<double> Atributes;
            bool write = false;
            bool write_name = false;
            for(int t{0}; t<tokens.size(); t++){
                if(t == 0){
                    name = tokens[t];
                    fPlanetList.push_back(tokens[0]);
                }
                if(t>0){
                    double inter = std::stod(tokens[t]);
                    Atributes.push_back(inter);
                }
            }
            fPlanetAtributesList.insert({name,Atributes});
        }
    } else if(file.find("Moons") != std::string::npos) {
        while (std::getline(inFile, x)) {
            if(x.find('#') != std::string::npos)
                continue;
            std::string Moon = "";
            std::string Planet = "";
            bool write = false;
            for (int i = 0; i < x.length(); i++) {
                // Print current character
                if(x[i-1] == '('){
                    Moon = x.substr(0, i-1);
                    write = true;
                }
                if(write){
                    if(x[i] == ')')
                        break;
                    Planet += x[i];
                }
            }
            fMoonsList.insert({Planet,Moon});
        }
    } else if(file.find("Satellite") != std::string::npos) {
        while (std::getline(inFile, x)) {
            if(x.find('#') != std::string::npos)
                continue;
            
            std::vector<std::string> tokens;
        
            std::istringstream iss(x);
            std::string s;
            while (iss >> s) {
                tokens.push_back(s);
            }
            
            std::string Sat = "";
            std::string name = "";
            std::string Planet = "";
            std::vector<double> Atributes;
            bool write = false;
            bool write_name = false;
            for(int t{0}; t<tokens.size(); t++){
                if(t == 0){
                    for (int i = 0; i < tokens[t].length(); i++) {
                        // Print current character
                        if(x[i-1] == '('){
                            Sat = tokens[t].substr(0, i-1);
                            write = true;
                            write_name = true;
                        }
                        if(write_name){
                            if(tokens[t][i] == ','){
                                write_name = false;
                                continue;
                            }
                            name += x[i];
                        }
                        if(write && !write_name){
                            if(tokens[t][i] == ')')
                                break;
                            Planet += x[i];
                        }
                    }
                    fSatelliteList[Planet].push_back(std::make_pair(Sat,name));
                }
                if(t>0){
                    double inter = std::stod(tokens[t]);
                    Atributes.push_back(inter);
                }
            }
            fSatelliteAtributesList.insert({name,Atributes});
        }
    }
}

void InputManager::ParseAsteroidFile(std::string file) {
    std::ifstream inFile(file);
    if (!inFile) {
        std::cout << "[InputManager] Asteroid list " << file << " not found — skipping." << std::endl;
        return;
    }
    std::cout << "[InputManager] Opening Config File: " << file << std::endl;

    std::string line;
    while (std::getline(inFile, line)) {
        if (line.empty() || line[0] == '#') continue;

        // Expected: name mass[kg] radius[m] PosX[AU] PosY[AU] PosZ[AU] VelX[m/s] VelY[m/s] VelZ[m/s]
        std::istringstream iss(line);
        std::vector<std::string> tokens;
        std::string tok;
        while (iss >> tok) tokens.push_back(tok);
        if (tokens.size() < 9) {
            std::cout << "[InputManager] Skipping malformed asteroid row: " << line << std::endl;
            continue;
        }

        MinorBody body;
        body.name   = tokens[0];
        body.mass   = std::stod(tokens[1]);
        body.radius = std::stod(tokens[2]);
        for (int k = 0; k < 3; ++k) body.pos[k] = std::stod(tokens[3 + k]);
        for (int k = 0; k < 3; ++k) body.vel[k] = std::stod(tokens[6 + k]);
        fAsteroids.push_back(body);
    }
}