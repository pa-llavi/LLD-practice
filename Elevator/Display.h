#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>
#include "Door.h"

enum class Direction;

class Display
{
    int elevatorId;

public:
    Display(int elevatorId);
    void showStatus(int currentFloor, Direction direction, DoorState doorState) const;

private:
    std::string directionToString(Direction direction) const;
    std::string doorstateToString(DoorState doorState) const;
};

#endif