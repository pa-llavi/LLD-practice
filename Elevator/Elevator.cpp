#include "Elevator.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>

Elevator::Elevator(int elevatorId, int minFloor, int maxFloor)
    : id(elevatorId), currentFloor(0), minFloor(minFloor), maxFloor(maxFloor), direction(Direction::IDLE)
{
    door = std::make_unique<Door>();
    display = std::make_unique<Display>(id);

    std::cout << "[Elevator " << id << "] Initialized at floor " << currentFloor << std::endl;
}

Elevator::~Elevator()
{
    std::cout << "[Elevator " << id << "] Shutting down..." << std::endl;
}

bool Elevator::addInternalRequest(int floor)
{
    if (floor < minFloor || floor > maxFloor)
    {
        std::cout << "[Elevator " << id << "] Invalid floor: " << floor << std::endl;
        return false;
    }

    if (floor == currentFloor)
    {
        std::cout << "[Elevator " << id << "] Already at floor: " << floor << std::endl;
        return false;
    }

    // Add to internal requests
    internalRequests.insert(floor);
    std::cout << "[Elevator " << id << "] Internal request added: Floor " << floor << std::endl;

    // If idle, determine initial direction
    if (direction == Direction::IDLE)
    {
        determineDirection();
    }

    return true;
}
void Elevator::moveToFloor(int targetFloor)
{
    if (targetFloor == currentFloor)
        return;

    // determine direction
    Direction moveDirection = (targetFloor > currentFloor) ? Direction::UP : Direction::DOWN;
    direction = moveDirection;

    std::cout << "[Elevator " << id << "] Moving from floor " << currentFloor
              << " to " << targetFloor
              << " (Direction: " << (direction == Direction::UP ? "UP" : "Down") << ")" << std::endl;

    // simulate movement
    while (currentFloor != targetFloor)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        if (direction == Direction::UP)
        {
            currentFloor++;
        }
        else
        {
            currentFloor--;
        }

        display->showStatus(currentFloor, direction, door->getState());
    }

    std::cout << "[Elevator " << id << "] Arrived at floor " << currentFloor << std::endl;

    handleDoorOperations();
}

bool Elevator::processNextRequest()
{
    if (internalRequests.empty())
    {
        direction = Direction::IDLE;
        std::cout << "[Elevator " << id << "] No requests. Status: IDLE" << std::endl;
        return false;
    }

    int nextFloor = -1;

    // For internal requests by default SCAN algorithm is used
    // SCAN algorithm: continue in current direction
    if (direction == Direction::UP || direction == Direction::IDLE)
    {
        // find next floor above current
        auto it = internalRequests.upper_bound(currentFloor);
        if (it != internalRequests.end())
        {
            nextFloor = *it;
            direction = Direction::UP;
        }
        else
        {
            // no request above, move down
            auto it2 = internalRequests.lower_bound(currentFloor);
            if (it2 != internalRequests.end())
            {
                --it2;
                nextFloor = *it2;
                direction = Direction::DOWN;
            }
        }
    }

    if (direction == Direction::DOWN)
    {
        // find next floor below current
        auto it = internalRequests.lower_bound(currentFloor);
        if (it != internalRequests.end())
        {
            --it;
            nextFloor = *it;
            // direction = Direction::DOWN;
        }
        else
        {
            auto it2 = internalRequests.upper_bound(currentFloor);
            if (it2 != internalRequests.end())
            {
                nextFloor = *it2;
                direction = Direction::UP;
            }
        }
    }

    if (nextFloor != -1)
    {
        moveToFloor(nextFloor);

        // remove internal request after completion
        internalRequests.erase(nextFloor);
        std::cout << "[Elevator " << id << "] Request completed for floor " << nextFloor << std::endl;
        determineDirection();

        return true;
    }

    direction = Direction::IDLE;
    return false;
}

void Elevator::openDoor()
{
    door->open();
}

void Elevator::closeDoor()
{
    door->close();
}

void Elevator::showStatus() const
{
    display->showStatus(currentFloor, direction, door->getState());

    if (!internalRequests.empty())
    {
        std::cout << " Pending requests: ";
        for (int floor : internalRequests)
        {
            std::cout << floor << " ";
        }
        std::cout << std::endl;
    }
}

int Elevator::calculateScore(int requestFloor, Direction requestDirection) const
{
    return 0;
}

void Elevator::determineDirection()
{
    if (internalRequests.empty())
    {
        direction = Direction::IDLE;
        return;
    }

    bool hasReqAbove = false;
    bool hasReqBelow = false;

    for (int floor : internalRequests)
    {
        if (floor > currentFloor)
        {
            hasReqAbove = true;
        }
        if (floor < currentFloor)
        {
            hasReqBelow = true;
        }
    }

    // if moving in a direction and has more req in that direction, then continue in that direction
    if (direction == Direction::UP && hasReqAbove)
        return;
    if (direction == Direction::DOWN && hasReqBelow)
        return;

    // change direction
    if (hasReqAbove)
    {
        direction = Direction::UP;
    }
    else if (hasReqBelow)
    {
        direction = Direction::DOWN;
    }
    else
    {
        direction = Direction::IDLE;
    }
}

void Elevator::handleDoorOperations()
{
    openDoor();

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    closeDoor();
}