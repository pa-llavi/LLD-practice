#include "../include/Ticket.h"
#include <iostream>

using namespace std;

// Static member initialization
int Ticket::ticketCounter = 0;

// ================= Constructor =================

Ticket::Ticket(Vehicle *v, ParkingSpot *s)
    : ticketID(++ticketCounter),
      vehicle(v),
      spot(s),
      amount(0),
      status(TicketStatus::ACTIVE)
{
    entryTime = time(nullptr);
    exitTime = 0;
}

// ================= Exit Time =================

void Ticket::setExitTime()
{
    exitTime = time(nullptr);
    status = TicketStatus::PENDING_PAYMENT;
}

// ================= Fee Calculation =================

double Ticket::calculateFee()
{
    if (exitTime == 0)
        setExitTime();

    double hours = difftime(exitTime, entryTime) / 3600.0;
    if (hours < 1)
        hours = 1;

    double rate = ParkingRate::getRate(vehicle->getVehicleType());
    amount = hours * rate;

    return amount;
}

// ================= Mark Paid =================

void Ticket::markPaid()
{
    status = TicketStatus::PAID;
}

// ================= Getters =================

int Ticket::getTicketID() const
{
    return ticketID;
}

Vehicle *Ticket::getParkedVehicle() const
{
    return vehicle;
}

ParkingSpot *Ticket::getParkingSpot() const
{
    return spot;
}

time_t Ticket::getEntryTime() const
{
    return entryTime;
}

time_t Ticket::getExitTime() const
{
    return exitTime;
}

double Ticket::getAmount() const
{
    return amount;
}

TicketStatus Ticket::getTicketStatus() const
{
    return status;
}

// ================= Display =================

void Ticket::displayTicket() const
{
    cout << "\n----------------------------------------\n";
    cout << "           Parking Ticket               \n";
    cout << "----------------------------------------\n";

    cout << " TicketID: " << ticketID << endl;
    cout << " Vehicle: " << vehicle->getLisencePlate() << endl;
    cout << " Vehicle Type: "
         << vehicleTypeToString(vehicle->getVehicleType()) << endl;
    cout << " Floor: " << spot->getFloorNumber() << endl;
    cout << " Spot: " << spot->getSpotID() << endl;

    cout << "----------------------------------------\n";
}
