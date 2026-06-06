#include "normfac.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

Double_t GetNormFactor(const std::string& filename) {
    std::ifstream infile(filename);
    std::string line;

    while (std::getline(infile, line)) {
        if (line.find("normfac") != std::string::npos) {
            std::stringstream ss(line);
            std::string temp;
            Double_t value;

            ss >> temp >> temp >> value;
            return value;
        }
    }

    std::cerr << "normfac not found in file!" << std::endl;
    return 0.0;
}
