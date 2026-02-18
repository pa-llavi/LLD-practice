#include "../include/Gate.h"
#include "../include/ParkingLot.h"

using namespace std;

// ================= EntranceGate =================

EntranceGate::EntranceGate(int id) : gateID(id) {}

int EntranceGate::getGateID() const
{
    return gateID;
}

Ticket *EntranceGate::processEntry(Vehicle *vehicle)
{
    cout << "Vehicle " << vehicle->getLisencePlate()
         << " entering through Gate "
         << gateID << endl;

    return ParkingLot::getInstance()->generateTicket(vehicle);
}

// ================= ExitGate =================

ExitGate::ExitGate(int id)
    : gateID(id), paymentProcessor(nullptr)
{
}

ExitGate::~ExitGate()
{
    delete paymentProcessor;
}

void ExitGate::setPaymentProcessor(PaymentProcessor *processor)
{
    paymentProcessor = processor;
}

int ExitGate::getGateID() const
{
    return gateID;
}

double ExitGate::processExit(Ticket *ticket)
{
    cout << "Vehicle exiting through Gate "
         << gateID << endl;

    if (paymentProcessor == nullptr)
    {
        paymentProcessor = new CashPayment();
    }

    return ParkingLot::getInstance()->processExit(ticket, paymentProcessor);
}
