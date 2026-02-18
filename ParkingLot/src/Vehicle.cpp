#include "../include/Vehicle.h"

using namespace std;

// ================= Base Constructor =================

Vehicle::Vehicle(const string &plate, VehicleType type)
    : licensePlate(plate), vehicleType(type)
{
}

// ================= Getters =================

string Vehicle::getLisencePlate() const
{
    return licensePlate;
}

VehicleType Vehicle::getVehicleType() const
{
    return vehicleType;
}

// ================= Display =================

void Vehicle::displayInfo() const
{
    cout << "Vehicle: "
         << vehicleTypeToString(vehicleType)
         << " | Plate: "
         << licensePlate << endl;
}

// ================= Derived Constructors =================

Bike::Bike(const string &plate)
    : Vehicle(plate, VehicleType::BIKE)
{
}

Car::Car(const string &plate)
    : Vehicle(plate, VehicleType::CAR)
{
}

Truck::Truck(const string &plate)
    : Vehicle(plate, VehicleType::TRUCK)
{
}
