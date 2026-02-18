#ifndef DOOR_H
#define DOOR_H

enum class DoorState
{
    OPEN,
    CLOSED,
    OPENING,
    CLOSING
};

class Door
{
    DoorState state;

public:
    Door();
    DoorState getState() const { return state; }
    bool isOpen() const { return state == DoorState::OPEN; }
    bool isClosed() const { return state == DoorState::CLOSED; }

    void open();
    void close();
    void emergencyOpen();

private:
    bool canOpen() const;
    bool canClose() const;
};

#endif