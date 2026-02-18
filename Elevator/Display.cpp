#include "Display.h"
#include "Elevator.h" // for Direction enum
#include <iostream>
#include <string>

Display::Display(int id) : elevatorId(id) {}

void Display::showStatus(int curFloor, Direction dir, DoorState state) const
{
    std::cout << "[Elevator " << elevatorId << " ]"
              << "Floor: " << curFloor << " | "
              << "Direction: " << directionToString(dir) << " | "
              << "Door: " << doorstateToString(state) << std::endl;
}

std::string Display::directionToString(Direction dir) const
{
    switch (dir)
    {
    case Direction::UP:
        return "UP";
    case Direction::DOWN:
        return "DOWN";
    case Direction::IDLE:
        return "IDLE";
    default:
        return "UNKNOWN";
    }
}

std::string Display::doorstateToString(DoorState state) const
{
    switch (state)
    {
    case DoorState::OPEN:
        return "OPEN";
    case DoorState::CLOSED:
        return "CLOSED";
    case DoorState::CLOSING:
        return "CLOSING";
    case DoorState::OPENING:
        return "OPENING";
    default:
        return "UNKNOWN";
    }
}