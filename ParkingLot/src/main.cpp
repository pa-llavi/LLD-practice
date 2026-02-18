#include "../include/ParkingLot.h"
#include "../include/Floor.h"
#include "../include/Gate.h"
#include "../include/ParkingSpot.h"
#include "../include/Vehicle.h"

#include <iomanip>
#include <thread>
using namespace std;

int main()
{
    cout << "╔═══════════════════════════════════════════════════════════╗" << endl;
    cout << "║          PARKING LOT SYSTEM - DEMONSTRATION              ║" << endl;
    cout << "╚═══════════════════════════════════════════════════════════╝\n"
         << endl;

    // 1. Create Parking Lot (Singleton)
    ParkingLot *parkingLot = ParkingLot::getInstance("City Center Parking");

    cout << "\n=== Setting up Parking Lot ===" << endl;

    // 2. Create and add floors
    Floor *floor1 = new Floor(1);
    Floor *floor2 = new Floor(2);

    parkingLot->addFloor(floor1);
    parkingLot->addFloor(floor2);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // 3. Add parking spots to Floor 1
    cout << "\n=== Adding spots to Floor 1 ===" << endl;
    for (int i = 1; i <= 5; i++)
    {
        floor1->addSpot(new SmallSpot(i, 1));
    }
    for (int i = 6; i <= 15; i++)
    {
        floor1->addSpot(new MediumSpot(i, 1));
    }
    for (int i = 16; i <= 20; i++)
    {
        floor1->addSpot(new LargeSpot(i, 1));
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
    // 4. Add parking spots to Floor 2
    cout << "\n=== Adding spots to Floor 2 ===" << endl;
    for (int i = 1; i <= 3; i++)
    {
        floor2->addSpot(new SmallSpot(i, 2));
    }
    for (int i = 4; i <= 13; i++)
    {
        floor2->addSpot(new MediumSpot(i, 2));
    }
    for (int i = 14; i <= 18; i++)
    {
        floor2->addSpot(new LargeSpot(i, 2));
    }

    // 5. Add gates
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cout << "\n=== Adding gates ===" << endl;
    EntranceGate *entrance1 = new EntranceGate(1);
    EntranceGate *entrance2 = new EntranceGate(2);
    ExitGate *exit1 = new ExitGate(1);
    ExitGate *exit2 = new ExitGate(2);

    parkingLot->addEntranceGate(entrance1);
    parkingLot->addEntranceGate(entrance2);
    parkingLot->addExitGate(exit1);
    parkingLot->addExitGate(exit2);

    // 6. Display parking rates
    std::this_thread::sleep_for(std::chrono::seconds(1));
    ParkingRate::displayRates();

    // 7. Display initial availability
    std::this_thread::sleep_for(std::chrono::seconds(1));
    parkingLot->displayAvailability();

    // 8. Simulate vehicle entry
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cout << "\n╔═══════════════════════════════════════════════════════════╗" << endl;
    cout << "║              SIMULATING VEHICLE ENTRIES                   ║" << endl;
    cout << "╚═══════════════════════════════════════════════════════════╝" << endl;

    // Create vehicles
    Car *car1 = new Car("ABC-1234");
    Car *car2 = new Car("XYZ-5678");
    Bike *bike1 = new Bike("BIKE-001");
    Truck *truck1 = new Truck("TRUCK-100");

    // Entry through gates
    std::this_thread::sleep_for(std::chrono::seconds(1));
    Ticket *ticket1 = entrance1->processEntry(car1);
    if (ticket1)
        ticket1->displayTicket();

    std::this_thread::sleep_for(std::chrono::seconds(1));
    Ticket *ticket2 = entrance1->processEntry(bike1);
    if (ticket2)
        ticket2->displayTicket();

    std::this_thread::sleep_for(std::chrono::seconds(1));
    Ticket *ticket3 = entrance2->processEntry(car2);
    if (ticket3)
        ticket3->displayTicket();

    std::this_thread::sleep_for(std::chrono::seconds(1));
    Ticket *ticket4 = entrance2->processEntry(truck1);
    if (ticket4)
        ticket4->displayTicket();

    // 9. Display updated availability
    std::this_thread::sleep_for(std::chrono::seconds(1));
    parkingLot->displayAvailability();

    // 10. Simulate vehicle exit
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cout << "\n╔═══════════════════════════════════════════════════════════╗" << endl;
    cout << "║              SIMULATING VEHICLE EXITS                     ║" << endl;
    cout << "╚═══════════════════════════════════════════════════════════╝" << endl;

    // Set up payment processors
    exit1->setPaymentProcessor(new CashPayment());
    exit2->setPaymentProcessor(new CardPayment());

    // Exit vehicles
    if (ticket1)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        double fee1 = exit1->processExit(ticket1);
        cout << "Total fee paid: $" << fixed << setprecision(2) << fee1 << endl;
    }

    if (ticket4)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        double fee4 = exit2->processExit(ticket4);
        cout << "Total fee paid: $" << fixed << setprecision(2) << fee4 << endl;
    }

    // 11. Display final availability
    std::this_thread::sleep_for(std::chrono::seconds(1));
    parkingLot->displayAvailability();

    // Cleanup
    delete car1;
    delete car2;
    delete bike1;
    delete truck1;

    ParkingLot::destroyInstance();

    cout << "\n✅ Parking Lot System demonstration complete!" << endl;

    return 0;
}