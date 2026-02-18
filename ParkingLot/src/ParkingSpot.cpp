#include "../include/ParkingSpot.h"
using namespace std;

ParkingSpot::ParkingSpot(int id, int floor, SpotType type) : spotID(id), floorNumber(floor), spotType(type),
                                                             isAvailable(true), parkedVehicle(nullptr) {}

bool ParkingSpot ::parkVehicle(Vehicle *vehicle)
{
    lock_guard<mutex> lock(spotMutex); // the desctuctors unlocks it automatically (when function returns)

    if (!isAvailable || !canFitVehicle(vehicle->getVehicleType()))
    {
        return false;
    }

    parkedVehicle = vehicle;
    isAvailable = false;
    cout << "Vehicle " << vehicle->getLisencePlate() << " parked at Spot: " << spotID << " on Floor: " << floorNumber << endl;
    return true;
}

bool ParkingSpot::unparkVehicle()
{
    lock_guard<mutex> lock(spotMutex);

    if (isAvailable)
        return false;

    cout << "Vehicle " << parkedVehicle->getLisencePlate() << " removed from spot " << spotID << endl;
    parkedVehicle = nullptr;
    isAvailable = true;
    return true;
}

int ParkingSpot::getSpotID() const
{
    return spotID;
}

int ParkingSpot::getFloorNumber() const { return floorNumber; }

bool ParkingSpot::getAvailability() const { return isAvailable; }

SpotType ParkingSpot::getSpotType() const { return spotType; }

Vehicle *ParkingSpot::getParkedVehicle() const { return parkedVehicle; }

SmallSpot::SmallSpot(int id, int floor)
    : ParkingSpot(id, floor, SpotType::SMALL)
{
}

MediumSpot::MediumSpot(int id, int floor)
    : ParkingSpot(id, floor, SpotType::MEDIUM)
{
}

LargeSpot::LargeSpot(int id, int floor)
    : ParkingSpot(id, floor, SpotType::LARGE)
{
}

// Small spot
bool SmallSpot::canFitVehicle(VehicleType type) const
{
    return type == VehicleType::BIKE;
}

// Medium spot
bool MediumSpot::canFitVehicle(VehicleType type) const
{
    return type == VehicleType::BIKE ||
           type == VehicleType::CAR;
}

// Large spot
bool LargeSpot::canFitVehicle(VehicleType type) const
{
    return true;
}