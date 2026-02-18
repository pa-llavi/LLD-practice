#pragma once

#include <mutex>
#include "Vehicle.h"
#include "Enumerations.h"

class ParkingSpot
{
protected:
    int spotID;
    int floorNumber;
    SpotType spotType;
    bool isAvailable;
    Vehicle *parkedVehicle;

    mutable std::mutex spotMutex;

public:
    ParkingSpot(int id, int floor, SpotType type);
    virtual ~ParkingSpot() = default;

    bool parkVehicle(Vehicle *vehicle);
    bool unparkVehicle();

    int getSpotID() const;
    int getFloorNumber() const;
    bool getAvailability() const;
    SpotType getSpotType() const;
    Vehicle *getParkedVehicle() const;

    virtual bool canFitVehicle(VehicleType type) const = 0;
};

class SmallSpot : public ParkingSpot
{
public:
    SmallSpot(int id, int floor);
    bool canFitVehicle(VehicleType type) const override;
};

class MediumSpot : public ParkingSpot
{
public:
    MediumSpot(int id, int floor);
    bool canFitVehicle(VehicleType type) const override;
};

class LargeSpot : public ParkingSpot
{
public:
    LargeSpot(int id, int floor);
    bool canFitVehicle(VehicleType type) const override;
};
