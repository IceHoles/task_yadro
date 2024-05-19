#include <iostream>
#include <fstream>
#include <string>
#include "club.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    std::ifstream inputFile(argv[1]);  
    //std::ifstream inputFile("test_file.txt");
    if (!inputFile) {
        std::cerr << "Error opening file: " << argv[1] << std::endl;
        return 1;
    }

    int totalTables;
    std::string startTime, endTime;
    int hourlyRate;
    inputFile >> totalTables >> startTime >> endTime >> hourlyRate;

    Club club(totalTables, startTime, endTime, hourlyRate);

    std::string time;
    int id;
    std::string clientName;
    int tableNumber;
    while (inputFile >> time >> id >> clientName) {
        if (id == 2) {
            inputFile >> tableNumber;
            club.processEvent(Event(time, id, clientName, tableNumber));
        }
        else {
            club.processEvent(Event(time, id, clientName));
        }
    }

    club.outputResults();

    return 0;
}