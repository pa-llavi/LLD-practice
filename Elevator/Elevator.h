#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <set>
#include <memory>
#include "Door.h"
#include "Display.h"

enum class Direction
{
    UP,
    DOWN,
    IDLE
};

class Elevator
{
private:
    int id;
    int currentFloor;
    int minFloor;
    int maxFloor;
    Direction direction;
    std::unique_ptr<Door> door;
    std::unique_ptr<Display> display;
    std::set<int> internalRequests;

public:
    Elevator(int elevatorId, int minFloor, int maxFloor);
    ~Elevator();

    // getters
    int getID() const { return id; }
    int getCurrentFloor() const { return currentFloor; }
    Direction getDirection() const { return direction; }
    bool isIdle() const { return direction == Direction::IDLE; }
    const std::set<int> &getInternalRequests() const { return internalRequests; }

    bool addInternalRequest(int floor);
    void moveToFloor(int targetFloor);
    bool processNextRequest();
    void openDoor();
    void closeDoor();
    void showStatus() const;
    int calculateScore(int requestFloor, Direction requestDirection) const;

private:
    void determineDirection();
    void handleDoorOperations();
};

#endif