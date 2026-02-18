#include "../include/DisplayBoard.h"

using namespace std;

DisplayBoard::DisplayBoard(int floor) : floorNumber(floor)
{
    availableSpots[SpotType::SMALL] = 0;
    availableSpots[SpotType::MEDIUM] = 0;
    availableSpots[SpotType::LARGE] = 0;
}

void DisplayBoard::update(SpotType type, int count)
{
    availableSpots[type] = count;
}

void DisplayBoard::display() const
{
    cout << "\n----------------------------------------\n";
    cout << "    FLOOR - " << floorNumber << " - Available Spots\n";
    cout << "----------------------------------------\n";
    cout << "     Bike: " << availableSpots.at(SpotType::SMALL) << endl;
    cout << "     Car: " << availableSpots.at(SpotType::MEDIUM) << endl;
    cout << "     Truck: " << availableSpots.at(SpotType::LARGE) << endl;
}
