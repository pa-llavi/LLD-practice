#pragma once

#include <map>
#include <iostream>
#include "../include/Enumerations.h"

class DisplayBoard
{
private:
    int floorNumber;
    std::map<SpotType, int> availableSpots;

public:
    DisplayBoard(int floor);

    void update(SpotType type, int count);
    void display() const;
};
