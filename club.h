#ifndef CLUB_H
#define CLUB_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <queue>
#include <vector>
#include <set>
#include <iomanip>

struct Event {
    std::string time;
    int id;
    std::string clientName;
    int tableNumber;

    Event(std::string t, int i, std::string cn, int tn = -1)
        : time(t), id(i), clientName(cn), tableNumber(tn) {}
};

class Club {
public:
    Club(int tables, std::string startTime, std::string endTime, int hourlyRate);

    void processEvent(const Event& event);
    void outputResults();

private:
    struct TableStatus {
        int revenue;
        int occupiedTime;
        bool isOccupied;
    };

    int totalTables;
    std::string startTime;
    std::string endTime;
    int hourlyRate;
    std::unordered_map<std::string, std::string> clients;
    std::unordered_map<std::string, int> clientsAtTable;
    std::queue<std::string> waitingQueue;
    std::unordered_map<int, TableStatus> tablesStatus;
    std::vector<std::string> eventLog;
    std::set<std::string> clientsInClub;

    void clientArrives(const Event& event);
    void clientSits(const Event& event);
    void clientWaits(const Event& event);
    void clientLeaves(const Event& event);
    std::string formatEvent(const Event& event, const std::string& message = "");
    int calculateTimeSpent(const std::string& start, const std::string& end);
};

#endif // CLUB_H
