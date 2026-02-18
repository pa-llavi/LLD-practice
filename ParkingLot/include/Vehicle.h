#pragma once

#include <string>
#include <iostream>
#include "Enumerations.h"

class Vehicle
{
protected:
    std::string licensePlate;
    VehicleType vehicleType;

public:
    Vehicle(const std::string &plate, VehicleType type);
    virtual ~Vehicle() = default;

    std::string getLisencePlate() const;
    VehicleType getVehicleType() const;

    virtual void displayInfo() const;
};

// ================= Derived Classes =================

class Bike : public Vehicle
{
public:
    Bike(const std::string &plate);
};

class Car : public Vehicle
{
public:
    Car(const std::string &plate);
};

class Truck : public Vehicle
{
public:
    Truck(const std::string &plate);
};
