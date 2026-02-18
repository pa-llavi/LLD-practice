#pragma once

#include <iostream>
#include "../include/Ticket.h"
#include "../include/Vehicle.h"
#include "../include/Payment.h"

// Forward declaration
class ParkingLot;

class EntranceGate
{
private:
    int gateID;

public:
    EntranceGate(int id);

    int getGateID() const;

    Ticket *processEntry(Vehicle *vehicle);
};

class ExitGate
{
private:
    int gateID;
    PaymentProcessor *paymentProcessor;

public:
    ExitGate(int id);
    ~ExitGate();

    void setPaymentProcessor(PaymentProcessor *processor);
    int getGateID() const;

    double processExit(Ticket *ticket);
};
