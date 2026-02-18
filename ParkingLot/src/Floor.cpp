#include "../include/Floor.h"

Floor::Floor(int number) : floorNumber(number)
{
    board = new DisplayBoard(number);
}

Floor::~Floor()
{
    for (auto spot : spots)
    {
        delete spot;
    }
    delete board;
}

bool Floor::addSpot(ParkingSpot *spot)
{
    spots.push_back(spot);
    updateDisplayBoard();
    return true;
}

ParkingSpot *Floor::findAvailableSpot(VehicleType vehicleType)
{
    SpotType preferredSpot;

    switch (vehicleType)
    {
    case VehicleType::BIKE:
        preferredSpot = SpotType::SMALL;
        break;
    case VehicleType::CAR:
        preferredSpot = SpotType::MEDIUM;
        break;
    case VehicleType::TRUCK:
        preferredSpot = SpotType::LARGE;
        break;
    }

    // Try preferred spot first
    for (auto spot : spots)
    {
        if (spot->getAvailability() &&
            spot->getSpotType() == preferredSpot)
        {
            return spot;
        }
    }

    // Try any fitting spot
    for (auto spot : spots)
    {
        if (spot->getAvailability() &&
            spot->canFitVehicle(vehicleType))
        {
            return spot;
        }
    }

    return nullptr;
}

int Floor::getAvailableSpotCount(SpotType type) const
{
    int count = 0;

    for (const auto &spot : spots)
    {
        if (spot->getSpotType() == type &&
            spot->getAvailability())
        {
            count++;
        }
    }

    return count;
}

void Floor::updateDisplayBoard()
{
    board->update(SpotType::SMALL,
                  getAvailableSpotCount(SpotType::SMALL));

    board->update(SpotType::MEDIUM,
                  getAvailableSpotCount(SpotType::MEDIUM));

    board->update(SpotType::LARGE,
                  getAvailableSpotCount(SpotType::LARGE));
}

void Floor::showDisplayBoard()
{
    board->display();
}

int Floor::getFloorNumber() const
{
    return floorNumber;
}

int Floor::getTotalSpots() const
{
    return spots.size();
}
