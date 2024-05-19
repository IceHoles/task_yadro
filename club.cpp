#include "club.h"

Club::Club(int tables, std::string startTime, std::string endTime, int hourlyRate)
    : totalTables(tables), startTime(startTime), endTime(endTime), hourlyRate(hourlyRate) {
    for (int i = 1; i <= totalTables; ++i) {
        tablesStatus[i] = { 0, 0, 0 };  // Initialize revenue, occupied time and occupied status for each table
    }
}

void Club::processEvent(const Event& event) {
    eventLog.push_back(formatEvent(event));
    if (event.id == 1) {
        clientArrives(event);
    }
    else if (event.id == 2) {
        clientSits(event);
    }
    else if (event.id == 3) {
        clientWaits(event);
    }
    else if (event.id == 4) {
        clientLeaves(event);
    }
}

void Club::outputResults() {
    std::cout << startTime << std::endl;
    for (auto it = clientsInClub.begin(); it != clientsInClub.end();) {
        clientLeaves(Event(endTime, 11, (it).operator*()));
        it = clientsInClub.begin();
    }
    for (const auto& event : eventLog) {
        std::cout << event << std::endl;
    }
    std::cout << endTime << std::endl;
    for (const auto& [table, status] : tablesStatus) {
        std::cout << table << " " << status.revenue << " " << std::setw(2) << std::setfill('0')
            << status.occupiedTime / 60 << ":" << std::setw(2) << std::setfill('0')
            << status.occupiedTime % 60 << std::endl;
    }
}

void Club::clientArrives(const Event& event) {
    if (clientsInClub.find(event.clientName) != clientsInClub.end()) {
        eventLog.push_back(std::string(event.time + " 13 YouShallNotPass"));
    }
    else if (event.time < startTime || event.time > endTime) {
        eventLog.push_back(std::string(event.time + " 13 NotOpenYet"));
    }
    else {
        clientsInClub.insert(event.clientName);
    }
}

void Club::clientSits(const Event& event) {
    if (clientsInClub.find(event.clientName) == clientsInClub.end()) {
        eventLog.push_back(std::string(event.time + " 13 ClientUnknown"));
    } 
    else if (tablesStatus[event.tableNumber].isOccupied) {
        eventLog.push_back(std::string(event.time + " 13 PlaceIsBusy"));
    }
    else if (clientsAtTable.find(event.clientName) != clientsAtTable.end()) {
        if (clientsAtTable[event.clientName] == event.tableNumber) {
            eventLog.push_back(std::string(event.time + " 13 PlaceIsBusy"));
        }
        else {
            int tableNumber = clientsAtTable[event.clientName];
            auto TimeSpent = calculateTimeSpent(clients[event.clientName], event.time);
            tablesStatus[tableNumber].occupiedTime += TimeSpent;
            tablesStatus[tableNumber].revenue += hourlyRate * (TimeSpent / 60 + 1);
            tablesStatus[tableNumber].isOccupied = 0;
            clientsAtTable[event.clientName] = event.tableNumber;
            tablesStatus[event.tableNumber].isOccupied = 1;
            clients.insert({ event.clientName, event.time });
        }
    }
    else {
        clientsAtTable[event.clientName] = event.tableNumber; 
        tablesStatus[event.tableNumber].isOccupied = 1;
        clients.insert({ event.clientName, event.time });
    }
}

void Club::clientWaits(const Event& event) {
    if (clientsInClub.find(event.clientName) == clientsInClub.end()) {
        eventLog.push_back(std::string(event.time + " 13 ClientUnknown"));
    }   
    else if (clientsAtTable.size() < 3) {
        eventLog.push_back(std::string(event.time + " 13 ICanWaitNoLonger!"));
    }
    else if (waitingQueue.size() >= totalTables) {
        clientLeaves(event);
    }
    else {
        waitingQueue.push(event.clientName);
    }
}

void Club::clientLeaves(const Event& event) {
    if (clientsInClub.find(event.clientName) == clientsInClub.end()) {
        eventLog.push_back(std::string(event.time + " 13 ClientUnknown"));
    }
    else {
        if (clientsAtTable.find(event.clientName) != clientsAtTable.end()) {
            int tableNumber = clientsAtTable[event.clientName];
            auto TimeSpent = calculateTimeSpent(clients[event.clientName], event.time);
            tablesStatus[tableNumber].occupiedTime += TimeSpent;
            tablesStatus[tableNumber].revenue += hourlyRate * (TimeSpent / 60 + 1);
            tablesStatus[tableNumber].isOccupied = 0;
            clientsAtTable.erase(event.clientName);
            if (!waitingQueue.empty()) {
                std::string nextClient = waitingQueue.front();
                waitingQueue.pop();
                clientsAtTable[nextClient] = tableNumber;
                eventLog.push_back(std::string(event.time + " 12 " + nextClient, tableNumber));
                clients.insert({ nextClient, event.time });
            }                              
            else { 
                eventLog.push_back(std::string(event.time + " 11 " + event.clientName)); 
            }
        }
        clientsInClub.erase(event.clientName);
        clients.erase(event.clientName);
    }
}

std::string Club::formatEvent(const Event& event, const std::string& message) {
    if (message.empty()) {
        return event.time + " " + std::to_string(event.id) + " " + event.clientName +
            (event.id == 2 ? " " + std::to_string(event.tableNumber) : "");
    }
    else {
        return event.time + " " + std::to_string(event.id) + " " + message;
    }
}

int Club::calculateTimeSpent(const std::string& start, const std::string& end) {
    int startHour = std::stoi(start.substr(0, 2));
    int startMin = std::stoi(start.substr(3, 2));
    int endHour = std::stoi(end.substr(0, 2));
    int endMin = std::stoi(end.substr(3, 2));
    return (endHour * 60 + endMin) - (startHour * 60 + startMin);
}
