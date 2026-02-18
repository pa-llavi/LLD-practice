#pragma once

#include <vector>
#include "../include/ParkingSpot.h"
#include "../include/DisplayBoard.h"

class Floor
{
private:
    int floorNumber;
    std::vector<ParkingSpot *> spots;
    DisplayBoard *board;

public:
    Floor(int number);
    ~Floor();

    bool addSpot(ParkingSpot *spot);
    ParkingSpot *findAvailableSpot(VehicleType vehicleType);

    int getAvailableSpotCount(SpotType type) const;
    void updateDisplayBoard();
    void showDisplayBoard();

    int getFloorNumber() const;
    int getTotalSpots() const;
};
