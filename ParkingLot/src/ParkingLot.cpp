#include "../include/ParkingLot.h"
#include <iostream>

using namespace std;

// Static member initialization
ParkingLot *ParkingLot::instance = nullptr;
mutex ParkingLot::instanceMutex;

ParkingLot::ParkingLot(const string &lotName) : name(lotName)
{
    ParkingRate::initialize();
    cout << "Parking Lot " << name << " created!\n";
}

ParkingLot::~ParkingLot()
{
    for (auto floor : floors)
        delete floor;

    for (auto gate : entranceGates)
        delete gate;

    for (auto gate : exitGates)
        delete gate;

    for (auto &[id, ticket] : activeTickets)
        delete ticket;
}

ParkingLot *ParkingLot::getInstance(const string &name)
{
    lock_guard<mutex> lock(instanceMutex);

    if (instance == nullptr)
        instance = new ParkingLot(name);

    return instance;
}

void ParkingLot::destroyInstance()
{
    lock_guard<mutex> lock(instanceMutex);
    delete instance;
    instance = nullptr;
}

bool ParkingLot::addFloor(Floor *floor)
{
    lock_guard<mutex> lock(lotMutex);
    floors.push_back(floor);
    cout << "Floor " << floor->getFloorNumber() << " added\n";
    return true;
}

bool ParkingLot::addEntranceGate(EntranceGate *gate)
{
    lock_guard<mutex> lock(lotMutex);
    entranceGates.push_back(gate);
    cout << "Entrance gate " << gate->getGateID() << " added\n";
    return true;
}

bool ParkingLot::addExitGate(ExitGate *gate)
{
    lock_guard<mutex> lock(lotMutex);
    exitGates.push_back(gate);
    cout << "Exit gate " << gate->getGateID() << " added\n";
    return true;
}

ParkingSpot *ParkingLot::findAvailableSpot(VehicleType type)
{
    for (auto floor : floors)
    {
        ParkingSpot *spot = floor->findAvailableSpot(type);
        if (spot != nullptr)
            return spot;
    }
    return nullptr;
}

Ticket *ParkingLot::generateTicket(Vehicle *vehicle)
{
    lock_guard<mutex> lock(lotMutex);

    ParkingSpot *spot = findAvailableSpot(vehicle->getVehicleType());
    if (!spot)
        return nullptr;

    if (!spot->parkVehicle(vehicle))
        return nullptr;

    Ticket *ticket = new Ticket(vehicle, spot);
    activeTickets[ticket->getTicketID()] = ticket;

    for (auto floor : floors)
        floor->updateDisplayBoard();

    return ticket;
}

double ParkingLot::processExit(Ticket *ticket, PaymentProcessor *processor)
{
    lock_guard<mutex> lock(lotMutex);

    if (!ticket)
        return -1;

    double fee = ticket->calculateFee();

    if (processor->processPayment(fee))
    {
        ticket->markPaid();
        ticket->getParkingSpot()->unparkVehicle();
        activeTickets.erase(ticket->getTicketID());

        for (auto floor : floors)
            floor->updateDisplayBoard();

        return fee;
    }

    return -1;
}

void ParkingLot::displayAvailability() const
{
    for (const auto &floor : floors)
        floor->showDisplayBoard();
}

string ParkingLot::getName() const
{
    return name;
}

int ParkingLot::getFloorCount() const
{
    return floors.size();
}
