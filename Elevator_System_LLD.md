# Elevator System - Low Level Design

## Table of Contents
1. [Overview](#overview)
2. [Design Choices & Rationale](#design-choices--rationale)
3. [Class Diagram](#class-diagram)
4. [Component Details](#component-details)
5. [Design Patterns Used](#design-patterns-used)
6. [SOLID Principles Applied](#solid-principles-applied)
7. [Algorithms](#algorithms)
8. [Thread Safety Considerations](#thread-safety-considerations)
9. [How to Compile and Run](#how-to-compile-and-run)

---

## Overview

This is a comprehensive Low Level Design for an Elevator System that supports:
- Multiple elevators in a building
- External requests (floor buttons) and internal requests (elevator buttons)
- Multiple scheduling algorithms (FCFS, SCAN, LOOK)
- Real-time elevator status monitoring
- Thread-safe operations

### Key Features
- **Scalable**: Supports multiple elevators and floors
- **Flexible**: Easy to add new scheduling algorithms
- **Maintainable**: Clear separation of concerns
- **Extensible**: Open for extension, closed for modification

---

## Design Choices & Rationale

### 1. **Enum Classes for Type Safety**
```cpp
enum class Direction { UP, DOWN, IDLE };
enum class DoorState { OPEN, CLOSED, OPENING, CLOSING };
```
**Rationale**: Enum classes provide type safety and prevent implicit conversions, making the code more robust.

### 2. **Separation of Request Types**
- **ExternalRequest**: Button presses outside the elevator (floor + direction)
- **InternalRequest**: Button presses inside the elevator (destination floor)

**Rationale**: External requests need a direction (UP/DOWN), while internal requests only need a destination. Separating them makes the design clearer and more maintainable.

### 3. **Strategy Pattern for Scheduling**
```cpp
class ElevatorScheduler {
public:
    virtual int selectElevator(const ExternalRequest& request) = 0;
};
```
**Rationale**: Different buildings may need different scheduling algorithms (FCFS, SCAN, LOOK, etc.). Strategy pattern allows switching algorithms at runtime without modifying the controller code.

### 4. **State Pattern Consideration**
While not fully implemented, the `Direction` enum represents elevator states (UP, DOWN, IDLE).

**Rationale**: Elevator behavior changes based on its state. State pattern can be extended to handle complex state transitions.

### 5. **Singleton Pattern for ElevatorController**
The controller manages all elevators and should have a single point of control.

**Rationale**: Only one controller should manage the elevator system to avoid conflicts and ensure consistent state.

### 6. **Encapsulation of Internal State**
Elevator's internal state (current floor, direction, door state) is private with controlled access.

**Rationale**: Prevents unauthorized state modifications and maintains invariants.

---

## Class Diagram

```
┌─────────────────────────┐
│  ElevatorController     │
│  (Singleton)            │
├─────────────────────────┤
│ - elevators: vector     │
│ - scheduler: unique_ptr │
│ - externalRequests: set │
├─────────────────────────┤
│ + addExternalRequest()  │
│ + processRequests()     │
│ + displayStatus()       │
└──────────┬──────────────┘
           │ manages
           │ 1..*
           ▼
┌─────────────────────────┐
│      Elevator           │
├─────────────────────────┤
│ - id: int               │
│ - currentFloor: int     │
│ - direction: Direction  │
│ - door: Door            │
│ - internalRequests: set │
├─────────────────────────┤
│ + addInternalRequest()  │
│ + moveToFloor()         │
│ + processNextRequest()  │
│ + openDoor()            │
│ + closeDoor()           │
└─────────────────────────┘
           │ has-a
           │ 1
           ▼
┌─────────────────────────┐
│        Door             │
├─────────────────────────┤
│ - state: DoorState      │
├─────────────────────────┤
│ + open()                │
│ + close()               │
│ + getState()            │
└─────────────────────────┘

┌─────────────────────────┐
│  ElevatorScheduler      │◄────────────┐
│  (Strategy Interface)   │             │
├─────────────────────────┤             │
│ + selectElevator()      │             │
└─────────────────────────┘             │
           ▲                            │
           │                            │
           │ implements                  │
    ┌──────┴──────────┬─────────────────┤
    │                 │                 │
┌───┴────────────┐ ┌──┴──────────────┐ ┌┴──────────────┐
│ FCFSScheduler  │ │ SCANScheduler   │ │ LOOKScheduler │
└────────────────┘ └─────────────────┘ └───────────────┘

┌─────────────────────────┐
│   ExternalRequest       │
├─────────────────────────┤
│ - floor: int            │
│ - direction: Direction  │
└─────────────────────────┘

┌─────────────────────────┐
│     Display             │
├─────────────────────────┤
│ - elevatorId: int       │
├─────────────────────────┤
│ + showStatus()          │
└─────────────────────────┘
```

---

## Component Details

### 1. **Elevator**
**Responsibility**: Represents a single elevator with its state and operations.

**Key Attributes**:
- `id`: Unique identifier
- `currentFloor`: Current position
- `direction`: Current movement direction
- `door`: Door object managing open/close state
- `internalRequests`: Set of destination floors

**Key Methods**:
- `addInternalRequest()`: Add floor destination
- `moveToFloor()`: Move elevator to a specific floor
- `processNextRequest()`: Process the next request based on direction

### 2. **Door**
**Responsibility**: Manages door state transitions.

**States**: OPEN, CLOSED, OPENING, CLOSING

**Why Separate Class?**: Door has its own lifecycle and can have complex logic (sensors, timing, safety mechanisms).

### 3. **ElevatorController**
**Responsibility**: Central manager for all elevators.

**Key Attributes**:
- `elevators`: Collection of all elevators
- `scheduler`: Scheduling strategy
- `externalRequests`: Queue of pending external requests

**Key Methods**:
- `addExternalRequest()`: Accept floor call
- `processRequests()`: Assign requests to elevators
- `displayStatus()`: Show system state

### 4. **ElevatorScheduler (Interface)**
**Responsibility**: Define scheduling algorithm interface.

**Implementations**:

#### a. **FCFSScheduler** (First Come First Serve)
- Assigns request to the first available elevator
- Simple but not optimal

#### b. **SCANScheduler** (Elevator Algorithm)
- Elevator continues in current direction until no more requests
- Then reverses direction
- Minimizes direction changes

#### c. **LOOKScheduler** (LOOK Algorithm)
- Similar to SCAN but doesn't go to extreme floors
- Reverses when no more requests in current direction
- More efficient than SCAN

### 5. **ExternalRequest**
**Responsibility**: Represents a call from a floor.

**Attributes**:
- `floor`: Floor number
- `direction`: Desired direction (UP/DOWN)

### 6. **Display**
**Responsibility**: Show elevator status to users.

---

## Design Patterns Used

### 1. **Strategy Pattern**
**Where**: ElevatorScheduler and its implementations
**Why**: Allows dynamic selection of scheduling algorithm without modifying controller code.

### 2. **Singleton Pattern**
**Where**: ElevatorController
**Why**: Ensures single point of control for the entire system.

### 3. **State Pattern (Implicit)**
**Where**: Direction enum and elevator behavior
**Why**: Elevator behavior changes based on its current state.

### 4. **Command Pattern (Implicit)**
**Where**: ExternalRequest and internal requests
**Why**: Encapsulates requests as objects, allowing queuing and logging.

---

## SOLID Principles Applied

### 1. **Single Responsibility Principle (SRP)**
- `Elevator`: Manages only elevator state and movement
- `Door`: Manages only door operations
- `ElevatorController`: Manages only system-level coordination
- `ElevatorScheduler`: Handles only scheduling logic

### 2. **Open/Closed Principle (OCP)**
- Adding new scheduling algorithms doesn't require modifying existing code
- Just create a new class implementing `ElevatorScheduler`

### 3. **Liskov Substitution Principle (LSP)**
- Any `ElevatorScheduler` implementation can be used interchangeably
- All schedulers follow the same contract

### 4. **Interface Segregation Principle (ISP)**
- `ElevatorScheduler` has a focused interface with only necessary methods
- Clients depend only on what they need

### 5. **Dependency Inversion Principle (DIP)**
- `ElevatorController` depends on `ElevatorScheduler` abstraction, not concrete implementations
- High-level controller doesn't depend on low-level scheduling details

---

## Algorithms

### 1. **FCFS (First Come First Serve)**
```
Algorithm:
1. Find first idle elevator
2. If no idle elevator, find nearest one
3. Assign request to selected elevator
```
**Time Complexity**: O(n) where n = number of elevators
**Space Complexity**: O(1)

### 2. **SCAN (Elevator Algorithm)**
```
Algorithm:
1. Elevator moves in one direction
2. Services all requests in that direction
3. When no more requests, reverses direction
4. Continues until all requests served
```
**Advantages**: Reduces direction changes, more predictable
**Disadvantages**: May cause starvation for extreme floors

### 3. **LOOK Algorithm**
```
Algorithm:
1. Similar to SCAN
2. Reverses direction when last request in current direction is served
3. Doesn't go to extreme floors if no requests
```
**Advantages**: More efficient than SCAN, reduces unnecessary movement
**Disadvantages**: Slightly more complex logic

---

## Thread Safety Considerations

### Potential Concurrency Issues:
1. **Multiple external requests simultaneously**
2. **Multiple elevators accessing shared controller state**
3. **Race conditions in request processing**

### Solutions (for production implementation):

1. **Mutex for Critical Sections**
```cpp
std::mutex elevatorMutex;
std::lock_guard<std::mutex> lock(elevatorMutex);
```

2. **Thread-Safe Containers**
```cpp
std::queue<ExternalRequest> requestQueue;
std::mutex queueMutex;
```

3. **Atomic Operations**
```cpp
std::atomic<int> currentFloor;
```

4. **Request Processing Thread**
- Separate thread for processing requests
- Main thread handles user input

---

## How to Compile and Run

### Compilation
```bash
# Compile all files
g++ -std=c++17 -o elevator_system \
    main.cpp \
    Elevator.cpp \
    Door.cpp \
    ElevatorController.cpp \
    ElevatorScheduler.cpp \
    Display.cpp

# Or use the provided Makefile
make

# Run
./elevator_system
```

### Example Usage
```cpp
// Create controller with 3 elevators, 10 floors, SCAN algorithm
ElevatorController* controller = ElevatorController::getInstance(3, 10);
controller->setScheduler(std::make_unique<SCANScheduler>());

// Add external request (Floor 5, going UP)
controller->addExternalRequest(5, Direction::UP);

// Process requests
controller->processRequests();

// Display status
controller->displayStatus();
```

---

## Future Enhancements

1. **Emergency Handling**: Fire mode, power failure
2. **Load Balancing**: Consider elevator capacity
3. **Maintenance Mode**: Take elevator out of service
4. **Energy Optimization**: Minimize power consumption
5. **Machine Learning**: Predict request patterns
6. **IoT Integration**: Real-time monitoring and analytics
7. **Access Control**: Security-based floor access
8. **Express Elevators**: Skip certain floors

---

## Testing Strategy

### Unit Tests
- Test individual elevator operations
- Test door state transitions
- Test scheduler algorithms

### Integration Tests
- Test controller with multiple elevators
- Test request processing flow
- Test concurrent requests

### Performance Tests
- Stress test with many requests
- Measure response time
- Test with different scheduling algorithms

---

## Complexity Analysis

### Space Complexity
- **Per Elevator**: O(F) where F = number of floors (for request set)
- **Controller**: O(E × F) where E = number of elevators
- **External Requests**: O(R) where R = pending requests

### Time Complexity
- **Add Request**: O(log F) for set insertion
- **Select Elevator**: O(E) for scheduler
- **Process Single Request**: O(1) for movement
- **Display Status**: O(E) to show all elevators

---

## Conclusion

This elevator system design demonstrates:
- **Clean Architecture**: Clear separation of concerns
- **Extensibility**: Easy to add new features
- **Maintainability**: Well-organized, documented code
- **Scalability**: Supports multiple elevators and floors
- **Best Practices**: SOLID principles, design patterns

The design balances simplicity with functionality, providing a solid foundation that can be extended for production use.
