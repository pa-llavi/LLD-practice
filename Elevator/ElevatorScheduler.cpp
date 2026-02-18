#include "ElevatorScheduler.h"
#include <limits>
#include <cmath>
#include <iostream>

// ---------------------- FCFS Scheduler ----------------------

int FCFSScheduler::selectElevator(const std::vector<std::unique_ptr<Elevator>> &elevators,
                                  const ExternalRequest &request)
{
    if (elevators.empty())
        return -1;

    int bestElevator = -1;
    int minDist = INT_MAX;

    // first, try to find an idle elevator
    for (size_t i = 0; i < elevators.size(); i++)
    {
        if (elevators[i]->isIdle())
        {
            int dist = std::abs(elevators[i]->getCurrentFloor() - request.floor);
            if (dist < minDist)
            {
                minDist = dist;
                bestElevator = i;
            }
        }
    }

    // if no idle elevator, find nearest one
    if (bestElevator == -1)
    {
        for (size_t i = 0; i < elevators.size(); i++)
        {
            int dist = std::abs(elevators[i]->getCurrentFloor() - request.floor);
            if (dist < minDist)
            {
                minDist = dist;
                bestElevator = i;
            }
        }
    }

    return bestElevator;
}

// ---------------------- SCAN Scheduler ----------------------

int SCANScheduler::selectElevator(const std::vector<std::unique_ptr<Elevator>> &elevators,
                                  const ExternalRequest &request)
{
    if (elevators.empty())
        return -1;

    int bestElevator = -1;
    int bestScore = INT_MAX;

    for (size_t i = 0; i < elevators.size(); i++)
    {
        const auto &elevator = elevators[i];
        int currFloor = elevator->getCurrentFloor();
        Direction currDirection = elevator->getDirection();

        int score = 0;
        int dist = std::abs(currFloor - request.floor);

        // Case 1: Elevator is idle - good candidate
        if (currDirection == Direction::IDLE)
        {
            score = dist * 10; // lower score id better
        }
        // Case 2: Elevator is moving in same direction
        else if (currDirection == request.direction)
        {
            if ((currDirection == Direction::UP && request.floor >= currFloor) ||
                (currDirection == Direction::DOWN && request.floor <= currFloor))
            {
                score = dist * 5; // Elevator is on the way
            }
            else
            {
                score = dist * 50; // Same direction but wrong side (floor already crossed)
            }
        }
        // Case 3: Elevator is moving in opposite direction
        else
        {
            score = dist * 30;
        }

        // add penalty for pending requests
        score += elevator->getInternalRequests().size() * 20;

        if (score < bestScore)
        {
            bestScore = score;
            bestElevator = i;
        }
    }

    return bestElevator;
}