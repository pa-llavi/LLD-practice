#pragma once

#include <string>
#include <vector>
#include <map>
#include <mutex>

#include "../include/ParkingRate.h"
#include "../include/ParkingSpot.h"
#include "../include/Ticket.h"
#include "../include/Floor.h"
#include "../include/Gate.h"

class ParkingLot
{
private:
    static ParkingLot *instance;
    static std::mutex instanceMutex;

    std::string name;
    std::vector<Floor *> floors;
    std::vector<EntranceGate *> entranceGates;
    std::vector<ExitGate *> exitGates;
    std::map<int, Ticket *> activeTickets;

    std::mutex lotMutex;

    ParkingLot(const std::string &lotName);

public:
    ParkingLot(const ParkingLot &) = delete;
    ParkingLot &operator=(const ParkingLot &) = delete;

    ~ParkingLot();

    static ParkingLot *getInstance(const std::string &name = "Main Parking Lot");
    static void destroyInstance();

    bool addFloor(Floor *floor);
    bool addEntranceGate(EntranceGate *gate);
    bool addExitGate(ExitGate *gate);

    ParkingSpot *findAvailableSpot(VehicleType type);
    Ticket *generateTicket(Vehicle *vehicle);
    double processExit(Ticket *ticket, PaymentProcessor *processor);

    void displayAvailability() const;

    std::string getName() const;
    int getFloorCount() const;
};
