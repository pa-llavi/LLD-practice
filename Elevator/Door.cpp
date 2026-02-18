#include "Door.h"
#include <iostream>
#include <thread>
#include <chrono>

Door::Door() : state(DoorState::CLOSED) {}

void Door::open()
{
    if (!canOpen())
    {
        std::cout << "    [Door] cannot open door\n";
        return;
    }

    if (state == DoorState::OPEN)
        return;

    state = DoorState::OPENING;
    std::cout << "    [Door] Opening...\n";

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    state = DoorState::OPEN;
    std::cout << "    [Door] Door is now OPEN\n";
}

void Door::close()
{
    if (!canClose())
    {
        std::cout << "    [Door] Cannot close door\n";
        return;
    }

    if (state == DoorState::CLOSED)
        return;

    state = DoorState::CLOSING;
    std::cout << "    [Door] Closing...\n";

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    state = DoorState::CLOSED;
    std::cout << "    [Door] Door is now CLOSED\n";
}

bool Door::canOpen() const
{
    // in actual production multiple cases needs to be tested like,
    // elevator is in IDLE state, aligned with a floor, emergency mode etc
    return (state == DoorState::CLOSED || state == DoorState::CLOSING);
}

bool Door::canClose() const
{
    // in actual production multiple cases needs to be tested like,
    // there is no obstacles, within weight limit, emergency mode etc
    return (state == DoorState::OPEN || state == DoorState::OPENING);
}