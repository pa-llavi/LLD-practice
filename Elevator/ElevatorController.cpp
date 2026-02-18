#include "ElevatorController.h"
#include <iostream>
#include <algorithm>

// Inatialize static member
ElevatorController *ElevatorController::instance = nullptr;

ElevatorController::ElevatorController(int numElevators, int minFloor, int maxFloor)
    : minFloor(minFloor), maxFloor(maxFloor)
{
    std::cout << "\n------------------------------------------\n";
    std::cout << "Initializing Elevator System\n";
    std::cout << "\n------------------------------------------\n";
    std::cout << "Number of Elevators: " << numElevators << std::endl;
    std::cout << "Floor Range: " << minFloor << " to " << maxFloor << std::endl;

    // create elevators
    for (int i = 0; i < numElevators; i++)
    {
        elevators.push_back(std::make_unique<Elevator>(i + 1, minFloor, maxFloor));
    }

    // Default scheduler: SCAN algorithm
    scheduler = std::make_unique<SCANScheduler>();
    std::cout << "Default Scheduler: " << scheduler->getName() << std::endl;
    std::cout << "\n------------------------------------------\n";
}

ElevatorController::~ElevatorController()
{
    std::cout << "\n[Controller] Shutting down elevator system....\n";
}

ElevatorController *ElevatorController::getInstance(int numElevators, int minFloor, int maxFloor)
{
    if (instance == nullptr)
    {
        instance = new ElevatorController(numElevators, minFloor, maxFloor);
    }
    return instance;
}

void ElevatorController::destroyInstance()
{
    if (instance != nullptr)
    {
        delete instance;
        instance = nullptr;
    }
}

void ElevatorController::setScheduler(std::unique_ptr<ElevatorScheduler> newScheduler)
{
    scheduler = std::move(newScheduler);
    std::cout << "[Controller] Scheduler changed to: " << scheduler->getName() << std::endl;
}

bool ElevatorController::addExternalRequest(int floor, Direction direction)
{
    if (!isValidFloor(floor))
    {
        std::cout << "[Controller] Invalid Floor: " << floor << std::endl;
        return false;
    }

    if (direction == Direction::IDLE)
    {
        std::cout << "[Controller] Invalid direction: IDLE\n";
        return false;
    }

    ExternalRequest request(floor, direction);
    externalRequests.insert(request);
    std::cout << "[Controller] External request added\n";
    return true;
}

void ElevatorController::processRequests()
{
    if (externalRequests.empty())
    {
        std::cout << "[Controller] No pending external requests" << std::endl;
        return;
    }

    std::cout << "\n========================================" << std::endl;
    std::cout << "Processing External Requests" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Pending requests: " << externalRequests.size() << std::endl;
    std::cout << "Scheduler: " << scheduler->getName() << std::endl;
    std::cout << "========================================\n"
              << std::endl;

    // Process each external request
    while (!externalRequests.empty())
    {
        // Get first request
        auto it = externalRequests.begin();
        ExternalRequest request = *it;

        // Process the request
        if (processExternalRequest(request))
        {
            // Remove from queue if successfully processed
            externalRequests.erase(it);
        }
        else
        {
            std::cout << "[Controller] Failed to process request, keeping in queue" << std::endl;
            break;
        }
    }

    std::cout << "\n[Controller] External request processing complete" << std::endl;
}

void ElevatorController::processInternalRequests()
{
    std::cout << "\n========================================" << std::endl;
    std::cout << "Processing Internal Requests" << std::endl;
    std::cout << "========================================\n"
              << std::endl;

    bool anyProcessed = false;

    // Process internal requests for each elevator
    for (auto &elevator : elevators)
    {
        while (elevator->processNextRequest())
        {
            anyProcessed = true;
        }
    }

    if (!anyProcessed)
    {
        std::cout << "[Controller] No internal requests to process" << std::endl;
    }

    std::cout << "\n[Controller] Internal request processing complete" << std::endl;
}

void ElevatorController::displayStatus() const
{
    std::cout << "\n========================================" << std::endl;
    std::cout << "Elevator System Status" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Scheduler: " << scheduler->getName() << std::endl;
    std::cout << "Pending External Requests: " << externalRequests.size() << std::endl;

    if (!externalRequests.empty())
    {
        std::cout << "External Request Queue: ";
        for (const auto &req : externalRequests)
        {
            std::cout << "Floor " << req.floor
                      << " (" << (req.direction == Direction::UP ? "UP" : "DOWN") << ") ";
        }
        std::cout << std::endl;
    }

    std::cout << "----------------------------------------" << std::endl;

    // Display each elevator's status
    for (const auto &elevator : elevators)
    {
        elevator->showStatus();
    }

    std::cout << "========================================\n"
              << std::endl;
}

Elevator *ElevatorController::getElevator(int index)
{
    if (index < 0 || index >= static_cast<int>(elevators.size()))
    {
        return nullptr;
    }
    return elevators[index].get();
}

bool ElevatorController::isValidFloor(int floor) const
{
    return (floor >= minFloor && floor <= maxFloor);
}

bool ElevatorController::processExternalRequest(const ExternalRequest &request)
{
    std::cout << "\n[Controller] Processing external request: Floor " << request.floor
              << " (" << (request.direction == Direction::UP ? "UP" : "DOWN") << ")" << std::endl;

    // Use scheduler to select best elevator
    int selectedIndex = scheduler->selectElevator(elevators, request);

    if (selectedIndex == -1)
    {
        std::cout << "[Controller] No available elevator for request" << std::endl;
        return false;
    }

    auto &selectedElevator = elevators[selectedIndex];
    std::cout << "[Controller] Assigned to Elevator " << selectedElevator->getID() << std::endl;

    // Add the floor as an internal request to the selected elevator
    selectedElevator->addInternalRequest(request.floor);

    return true;
}
