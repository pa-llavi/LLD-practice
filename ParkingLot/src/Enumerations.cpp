#include "../include/Enumerations.h"

std::string vehicleTypeToString(VehicleType type)
{
    switch (type)
    {
    case VehicleType::BIKE:
        return "Bike";
    case VehicleType::CAR:
        return "Car";
    case VehicleType::TRUCK:
        return "Truck";
    default:
        return "Unknown";
    }
}

std::string spotTypeToString(SpotType type)
{
    switch (type)
    {
    case SpotType::SMALL:
        return "Small";
    case SpotType::MEDIUM:
        return "Medium";
    case SpotType::LARGE:
        return "Large";
    default:
        return "Unknown";
    }
}
