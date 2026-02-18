#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include "ElevatorController.h"
#include "ElevatorScheduler.h"

/**
 * @file main.cpp
 * @brief Demonstration of the Elevator System
 *
 * This file demonstrates:
 * 1. System initialization
 * 2. External requests (floor button presses)
 * 3. Internal requests (elevator button presses)
 * 4. Different scheduling algorithms
 * 5. Real-time status monitoring
 */

void printMenu()
{
    std::cout << "\n========================================" << std::endl;
    std::cout << "      Elevator System Control Menu      " << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "1. Add External Request (Floor Button)" << std::endl;
    std::cout << "2. Add Internal Request (Elevator Button)" << std::endl;
    std::cout << "3. Process External Requests" << std::endl;
    std::cout << "4. Process Internal Requests" << std::endl;
    std::cout << "5. Display System Status" << std::endl;
    std::cout << "6. Change Scheduler Algorithm" << std::endl;
    std::cout << "7. Run Demo Scenario" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Enter choice: ";
}

void runDemoScenario(ElevatorController *controller)
{
    std::cout << "\n========================================" << std::endl;
    std::cout << "      Running Demo Scenario             " << std::endl;
    std::cout << "========================================" << std::endl;

    // Scenario: Multiple people calling elevators from different floors

    std::cout << "\n--- Scenario 1: Multiple External Requests ---" << std::endl;
    controller->addExternalRequest(3, Direction::UP);   // Person at floor 3 wants to go up
    controller->addExternalRequest(7, Direction::DOWN); // Person at floor 7 wants to go down
    controller->addExternalRequest(5, Direction::UP);   // Person at floor 5 wants to go up

    std::cout << "\nProcessing external requests..." << std::endl;
    controller->processRequests();

    std::cout << "\n--- Scenario 2: Adding Internal Requests ---" << std::endl;
    // Simulate people inside elevators pressing buttons
    Elevator *elevator1 = controller->getElevator(0);
    if (elevator1)
    {
        elevator1->addInternalRequest(8);  // Person wants to go to floor 8
        elevator1->addInternalRequest(10); // Another person wants to go to floor 10
    }

    Elevator *elevator2 = controller->getElevator(1);
    if (elevator2)
    {
        elevator2->addInternalRequest(2); // Person wants to go to floor 2
        elevator2->addInternalRequest(1); // Another person wants to go to floor 1
    }

    std::cout << "\nProcessing internal requests..." << std::endl;
    controller->processInternalRequests();

    std::cout << "\n--- Scenario 3: Rush Hour Simulation ---" << std::endl;
    // Multiple people on ground floor going up
    controller->addExternalRequest(0, Direction::UP);
    controller->addExternalRequest(1, Direction::UP);
    controller->addExternalRequest(2, Direction::UP);
    controller->addExternalRequest(9, Direction::DOWN);
    controller->addExternalRequest(8, Direction::DOWN);

    std::cout << "\nProcessing rush hour requests..." << std::endl;
    controller->processRequests();

    // Add destinations
    for (int i = 0; i < controller->getNumElevators(); i++)
    {
        Elevator *elev = controller->getElevator(i);
        if (elev)
        {
            elev->addInternalRequest(6);
            elev->addInternalRequest(4);
        }
    }

    controller->processInternalRequests();

    std::cout << "\n--- Demo Scenario Complete ---" << std::endl;
    controller->displayStatus();
}

void changeScheduler(ElevatorController *controller)
{
    std::cout << "\n========================================" << std::endl;
    std::cout << "      Select Scheduling Algorithm       " << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "1. FCFS (First Come First Serve)" << std::endl;
    std::cout << "2. SCAN (Elevator Algorithm)" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Enter choice: ";

    int choice;
    std::cin >> choice;

    switch (choice)
    {
    case 1:
        controller->setScheduler(std::make_unique<FCFSScheduler>());
        break;
    case 2:
        controller->setScheduler(std::make_unique<SCANScheduler>());
        break;
    default:
        std::cout << "Invalid choice! Keeping current scheduler." << std::endl;
    }
}

int main()
{
    std::cout << "\n╔════════════════════════════════════════╗" << std::endl;
    std::cout << "║   ELEVATOR SYSTEM - LOW LEVEL DESIGN   ║" << std::endl;
    std::cout << "╚════════════════════════════════════════╝\n"
              << std::endl;

    // Initialize the elevator system
    // 3 elevators, floors 0 to 10
    ElevatorController *controller = ElevatorController::getInstance(3, 0, 10);

    // Set default scheduler (LOOK algorithm)
    controller->setScheduler(std::make_unique<SCANScheduler>());

    // Display initial status
    controller->displayStatus();

    // Interactive menu
    int choice;
    do
    {
        printMenu();
        std::cin >> choice;

        switch (choice)
        {
        case 1:
        {
            // Add external request
            int floor;
            char dir;
            std::cout << "Enter floor number: ";
            std::cin >> floor;
            std::cout << "Enter direction (U for UP, D for DOWN): ";
            std::cin >> dir;

            Direction direction = (dir == 'U' || dir == 'u') ? Direction::UP : Direction::DOWN;
            controller->addExternalRequest(floor, direction);
            break;
        }

        case 2:
        {
            // Add internal request
            int elevatorId, floor;
            std::cout << "Enter elevator ID (1-" << controller->getNumElevators() << "): ";
            std::cin >> elevatorId;
            std::cout << "Enter destination floor: ";
            std::cin >> floor;

            Elevator *elevator = controller->getElevator(elevatorId - 1);
            if (elevator)
            {
                elevator->addInternalRequest(floor);
            }
            else
            {
                std::cout << "Invalid elevator ID!" << std::endl;
            }
            break;
        }

        case 3:
            // Process external requests
            controller->processRequests();
            break;

        case 4:
            // Process internal requests
            controller->processInternalRequests();
            break;

        case 5:
            // Display status
            controller->displayStatus();
            break;

        case 6:
            // Change scheduler
            changeScheduler(controller);
            break;

        case 7:
            // Run demo scenario
            runDemoScenario(controller);
            break;

        case 0:
            std::cout << "\nExiting system..." << std::endl;
            break;

        default:
            std::cout << "Invalid choice! Please try again." << std::endl;
        }

    } while (choice != 0);

    // Cleanup
    ElevatorController::destroyInstance();

    std::cout << "\n╔════════════════════════════════════════╗" << std::endl;
    std::cout << "║         System Shutdown Complete       ║" << std::endl;
    std::cout << "╚════════════════════════════════════════╝\n"
              << std::endl;

    return 0;
}