#ifndef ELEVATOR_SCHEDULER_H
#define ELEVATOR_SCHEDULER_H

#include <vector>
#include <memory>
#include "Elevator.h"

struct ExternalRequest
{
    int floor;
    Direction direction;

    ExternalRequest(int f, Direction d) : floor(f), direction(d) {}

    // for use in std::set (ordering)
    bool operator<(const ExternalRequest &other) const
    {
        if (floor != other.floor)
            return floor < other.floor;
        return direction < other.direction;
    }
};

class ElevatorScheduler
{
public:
    virtual ~ElevatorScheduler() = default;

    virtual int selectElevator(const std::vector<std::unique_ptr<Elevator>> &elevators,
                               const ExternalRequest &request) = 0;

    virtual std::string getName() const = 0;
};

class FCFSScheduler : public ElevatorScheduler
{
public:
    int selectElevator(const std::vector<std::unique_ptr<Elevator>> &elevators,
                       const ExternalRequest &request) override;
    std::string getName() const override { return "FCFS (First Come First Serve)"; }
};

class SCANScheduler : public ElevatorScheduler
{
public:
    int selectElevator(const std::vector<std::unique_ptr<Elevator>> &elevators,
                       const ExternalRequest &request) override;
    std::string getName() const override { return "SCAN Algorithm"; }
};

#endif