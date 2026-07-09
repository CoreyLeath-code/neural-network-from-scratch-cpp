#include "CSVLoader.hpp"
#include <fstream>
#include <sstream>

std::vector<std::vector<double>> loadCSV(const std::string& path) {
    std::ifstream file(path);
    std::vector<std::vector<double>> data;
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;
        std::vector<double> row;
        while (std::getline(ss, cell, ',')) {
            row.push_back(std::stod(cell));
        }
        data.push_back(row);
    }
    return data;
}
