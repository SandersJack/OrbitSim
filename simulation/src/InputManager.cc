#include "InputManager.hh"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

InputManager::InputManager() {

}

InputManager *InputManager::GetInstance() {
    static InputManager instance;
    return &instance;
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
            
            std::string name;
            std::vector<double> Atributes;
            for(std::size_t t = 0; t < tokens.size(); t++){
                if(t == 0){
                    name = tokens[t];
                    fPlanetList.push_back(tokens[0]);
                } else {
                    Atributes.push_back(std::stod(tokens[t]));
                }
            }
            fPlanetAtributesList.insert({name,Atributes});
        }
    } else if(file.find("Moons") != std::string::npos) {
        // Each line is "Moon(Planet)".
        while (std::getline(inFile, x)) {
            if(x.find('#') != std::string::npos)
                continue;
            std::size_t open  = x.find('(');
            std::size_t close = x.find(')');
            if(open == std::string::npos || close == std::string::npos || close < open)
                continue;
            std::string Moon   = x.substr(0, open);
            std::string Planet = x.substr(open + 1, close - open - 1);
            fMoonsList.insert({Planet,Moon});
        }
    } else if(file.find("Satellite") != std::string::npos) {
        // First token is "Type(Name,Planet)"; the rest are numeric attributes.
        while (std::getline(inFile, x)) {
            if(x.find('#') != std::string::npos)
                continue;

            std::vector<std::string> tokens;
            std::istringstream iss(x);
            std::string s;
            while (iss >> s) {
                tokens.push_back(s);
            }
            if(tokens.empty())
                continue;

            const std::string &head = tokens[0];
            std::size_t open  = head.find('(');
            std::size_t comma = head.find(',');
            std::size_t close = head.find(')');
            if(open == std::string::npos || comma == std::string::npos ||
               close == std::string::npos)
                continue;

            std::string Sat    = head.substr(0, open);
            std::string name   = head.substr(open + 1, comma - open - 1);
            std::string Planet = head.substr(comma + 1, close - comma - 1);
            fSatelliteList[Planet].push_back(std::make_pair(Sat, name));

            std::vector<double> Atributes;
            for(std::size_t t = 1; t < tokens.size(); t++){
                Atributes.push_back(std::stod(tokens[t]));
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