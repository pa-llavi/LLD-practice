#pragma once

#include <string>

enum class VehicleType
{
    BIKE,
    CAR,
    TRUCK
};

enum class SpotType
{
    SMALL,
    MEDIUM,
    LARGE
};

enum class TicketStatus
{
    ACTIVE,
    CREATED,
    PENDING_PAYMENT,
    PAID
};

// Helper functions
std::string vehicleTypeToString(VehicleType type);
std::string spotTypeToString(SpotType type);
