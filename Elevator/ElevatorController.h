#ifndef ELEVATAOR_CONTROLLER_H
#define ELEVATAOR_CONTROLLER_H

#include <vector>
#include <set>
#include <memory>
#include "Elevator.h"
#include "ElevatorScheduler.h"

class ElevatorController
{
    static ElevatorController *instance; // singleton instance

    std::vector<std::unique_ptr<Elevator>> elevators; // all elevators
    std::set<ExternalRequest> externalRequests;       // pending external requests
    std::unique_ptr<ElevatorScheduler> scheduler;     // scheduling strategy

    int minFloor;
    int maxFloor;

    ElevatorController(int numElevators, int minFloor, int maxFloor);

public:
    ~ElevatorController();

    // delete copy constructor and assignment operator(singleton)
    ElevatorController(const ElevatorController &) = delete;
    ElevatorController &operator=(const ElevatorController &) = delete;

    static ElevatorController *getInstance(int numElevators = 3, int minFloor = 0, int maxFloor = 10);

    static void destroyInstance();
    void setScheduler(std::unique_ptr<ElevatorScheduler> newScheduler);
    bool addExternalRequest(int floor, Direction direction);
    void processRequests();
    void processInternalRequests();
    void displayStatus() const;
    int getNumElevators() const { return elevators.size(); }
    Elevator *getElevator(int id);
    int getPendingRequestsCount() const { return externalRequests.size(); }

private:
    bool isValidFloor(int floor) const;
    bool processExternalRequest(const ExternalRequest &request);
};

#endif