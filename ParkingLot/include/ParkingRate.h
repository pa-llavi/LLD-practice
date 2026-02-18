#pragma once
#include <map>
#include <mutex>
#include "Enumerations.h"

class ParkingRate
{
private:
    static std::map<VehicleType, double> hourlyRates;
    static std::mutex rateMutex;

public:
    static void initialize();
    static double getRate(VehicleType type);
    static void setRate(VehicleType type, double rate);
    static void displayRates();
};
