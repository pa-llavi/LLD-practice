#include "../include/ParkingRate.h"
#include <iostream>

std::map<VehicleType, double> ParkingRate::hourlyRates;
std::mutex ParkingRate::rateMutex;

void ParkingRate::initialize()
{
    hourlyRates[VehicleType::BIKE] = 10;
    hourlyRates[VehicleType::CAR] = 20;
    hourlyRates[VehicleType::TRUCK] = 30;
}

double ParkingRate::getRate(VehicleType type)
{
    std::lock_guard<std::mutex> lock(rateMutex);
    return hourlyRates[type];
}

void ParkingRate::setRate(VehicleType type, double rate)
{
    std::lock_guard<std::mutex> lock(rateMutex);
    hourlyRates[type] = rate;
}

void ParkingRate::displayRates()
{
    std::cout << "Rates configured\n";
}
