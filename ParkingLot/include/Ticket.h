#pragma once

#include <ctime>
#include "Vehicle.h"
#include "ParkingSpot.h"
#include "ParkingRate.h"
#include "Enumerations.h"

class Ticket
{
private:
    static int ticketCounter;

    int ticketID;
    Vehicle *vehicle;
    ParkingSpot *spot;

    time_t entryTime;
    time_t exitTime;

    double amount;
    TicketStatus status;

public:
    Ticket(Vehicle *v, ParkingSpot *s);

    void setExitTime();
    double calculateFee();
    void markPaid();

    // getters
    int getTicketID() const;
    Vehicle *getParkedVehicle() const;
    ParkingSpot *getParkingSpot() const;
    time_t getEntryTime() const;
    time_t getExitTime() const;
    double getAmount() const;
    TicketStatus getTicketStatus() const;

    void displayTicket() const;
};
