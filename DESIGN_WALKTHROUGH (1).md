# Elevator System - Complete Design Walkthrough

## 🎯 Overview
This document provides a complete walkthrough of the elevator system design, explaining every design decision, pattern choice, and implementation detail.

---

## 📊 System Architecture

### High-Level Architecture
```
┌─────────────────────────────────────────────────────┐
│                    User/Client                      │
│              (Presses buttons, views status)        │
└────────────────────┬────────────────────────────────┘
                     │
                     ▼
┌─────────────────────────────────────────────────────┐
│              ElevatorController                     │
│                  (Singleton)                        │
│  - Manages all elevators                            │
│  - Queues external requests                         │
│  - Delegates to scheduler                           │
└────────────────────┬────────────────────────────────┘
                     │
        ┌────────────┴────────────┐
        ▼                         ▼
┌──────────────────┐    ┌──────────────────┐
│  Scheduler       │    │  Elevators       │
│  (Strategy)      │    │  (Collection)    │
│                  │    │                  │
│  - FCFS          │    │  - Elevator 1    │
│  - SCAN          │    │  - Elevator 2    │
│  - LOOK          │    │  - Elevator 3    │
└──────────────────┘    └────────┬─────────┘
                                 │
                    ┌────────────┴────────────┐
                    ▼                         ▼
              ┌──────────┐            ┌───────────┐
              │   Door   │            │  Display  │
              └──────────┘            └───────────┘
```

---

## 🏗️ Component-by-Component Walkthrough

### 1. Door Component

**Purpose**: Manages the lifecycle and state of elevator doors.

**Design Decision**: Separate class instead of just a boolean flag.

**Why?**
- Door has its own complex lifecycle (OPENING → OPEN → CLOSING → CLOSED)
- Can be extended with sensors, obstruction detection
- In real systems, doors are separate physical components
- Follows Single Responsibility Principle

**State Machine**:
```
CLOSED ──[open()]──> OPENING ──[time]──> OPEN
  ▲                                        │
  │                                        │
  └──────[time]──── CLOSING ◄──[close()]──┘
```

**Key Implementation Details**:
```cpp
class Door {
private:
    DoorState state;  // Current state
    
public:
    void open() {
        state = DoorState::OPENING;
        // Simulate opening time
        sleep(500ms);
        state = DoorState::OPEN;
    }
    
    void close() {
        state = DoorState::CLOSING;
        // Simulate closing time
        sleep(500ms);
        state = DoorState::CLOSED;
    }
};
```

**Extensibility**:
- Add sensor checks in `canOpen()` and `canClose()`
- Add obstruction detection
- Add emergency override
- Add timeout handling

---

### 2. Display Component

**Purpose**: Shows elevator status to users.

**Design Decision**: Separate class for display logic.

**Why?**
- Decouples presentation from business logic
- Easy to swap implementations (LED, LCD, Web UI)
- Makes testing easier (can mock display)
- Follows Single Responsibility Principle

**Key Implementation Details**:
```cpp
class Display {
public:
    void showStatus(int floor, Direction dir, DoorState doorState) {
        // Format and display information
        cout << "Floor: " << floor 
             << " | Direction: " << directionToString(dir)
             << " | Door: " << doorStateToString(doorState);
    }
};
```

**Why not just print directly from Elevator?**
- In production, display might be network-connected
- May need to support multiple display types
- Easier to test Elevator without display
- Can add display buffering, animations, etc.

---

### 3. Elevator Component

**Purpose**: Represents a single elevator with all its state and behavior.

**Design Decision**: Core domain entity with encapsulated state.

**State Management**:
```cpp
class Elevator {
private:
    int id;                         // Unique identifier
    int currentFloor;               // Current position
    Direction direction;            // UP, DOWN, or IDLE
    std::set<int> internalRequests; // Destination floors (sorted)
    std::unique_ptr<Door> door;     // Owns door
    std::unique_ptr<Display> display; // Owns display
};
```

**Why `std::set` for requests?**
1. **Automatic Sorting**: Requests are always in floor order
2. **No Duplicates**: Can't request same floor twice
3. **Efficient Lookup**: O(log n) for finding next floor
4. **Clean Code**: No manual sorting needed

Example:
```cpp
internalRequests = {2, 5, 8, 10}  // Automatically sorted
// If at floor 3 going UP, next stop is 5
// If at floor 6 going DOWN, next stop is 5
```

**Movement Algorithm (SCAN-like)**:
```cpp
bool processNextRequest() {
    if (direction == Direction::UP) {
        // Find next floor above current
        auto it = internalRequests.upper_bound(currentFloor);
        if (it != internalRequests.end()) {
            moveToFloor(*it);  // Continue going up
        } else {
            // No requests above, check below
            direction = Direction::DOWN;
        }
    }
    // Similar logic for DOWN
}
```

**Why this algorithm?**
- Minimizes direction changes
- More predictable for passengers
- Efficient for multiple stops
- Standard in real elevator systems

**Scoring System for Scheduler**:
```cpp
int calculateScore(int requestFloor, Direction requestDirection) {
    int score = 0;
    int distance = abs(requestFloor - currentFloor);
    
    if (direction == IDLE) {
        score = distance;  // Best case: idle and nearby
    }
    else if (direction == requestDirection) {
        if (onTheWayToRequest) {
            score = distance;  // Good: on the way
        } else {
            score = 1000 + distance;  // Bad: wrong side
        }
    }
    else {
        score = 500 + distance;  // Okay: opposite direction
    }
    
    score += internalRequests.size() * 10;  // Penalty for busy
    
    return score;  // Lower is better
}
```

---

### 4. Request Types

**Design Decision**: Two separate request types.

**Why not use a single Request class?**

**External Request**:
```cpp
struct ExternalRequest {
    int floor;         // Where button was pressed
    Direction direction;  // UP or DOWN
};
```
- Represents floor button presses
- **Must specify direction** (UP or DOWN button)
- Example: Person at floor 5 wants to go UP

**Internal Request**:
```cpp
// Just an integer (destination floor)
internalRequests.insert(8);  // Person inside wants floor 8
```
- Represents buttons inside elevator
- **Only needs destination** (direction is implicit)
- Example: Person inside going to floor 8

**Why separate?**
- Different data requirements
- Different validation rules
- Clearer intent in code
- Prevents mistakes (can't add direction to internal request)

---

### 5. Scheduling Algorithms (Strategy Pattern)

**Design Pattern**: Strategy Pattern

**Structure**:
```cpp
// Base class (Strategy Interface)
class ElevatorScheduler {
public:
    virtual int selectElevator(
        const vector<unique_ptr<Elevator>>& elevators,
        const ExternalRequest& request
    ) = 0;
};

// Concrete strategies
class FCFSScheduler : public ElevatorScheduler { /* ... */ };
class SCANScheduler : public ElevatorScheduler { /* ... */ };
class LOOKScheduler : public ElevatorScheduler { /* ... */ };
```

**Why Strategy Pattern?**
1. **Open/Closed Principle**: Add new algorithms without modifying existing code
2. **Runtime Switching**: Change algorithm on-the-fly
3. **Testing**: Test each algorithm independently
4. **Comparison**: Easy to compare performance

**Algorithm Comparison**:

#### FCFS (First Come First Serve)
```
Algorithm:
1. Find idle elevator
2. If none idle, find nearest
3. Assign to that elevator

Pros: Simple, fair
Cons: Not optimal

Example:
Request at floor 7 going UP
Elevator 1: Floor 3, IDLE → distance = 4
Elevator 2: Floor 8, IDLE → distance = 1
Select: Elevator 2 (nearest)
```

#### SCAN (Elevator Algorithm)
```
Algorithm:
1. Prefer elevators moving towards request
2. Consider direction and distance
3. Penalize opposite direction

Scoring:
- Same direction, on the way: score = distance * 5
- Same direction, wrong side: score = distance * 50
- Opposite direction: score = distance * 30
- Idle: score = distance * 10

Example:
Request at floor 7 going UP
Elevator 1: Floor 5, UP → score = (7-5) * 5 = 10
Elevator 2: Floor 9, DOWN → score = (9-7) * 30 = 60
Select: Elevator 1 (better score)
```

#### LOOK Algorithm (Default, Best)
```
Algorithm:
1. Use elevator's calculateScore() method
2. Consider direction, distance, load
3. Most sophisticated scoring

Scoring:
- Idle: distance
- Same direction, on way: distance
- Same direction, wrong side: 1000 + distance
- Opposite direction: 500 + distance
- Add penalty: requests * 10

Example:
Request at floor 7 going UP
Elevator 1: Floor 5, UP, 2 requests → score = 2 + 20 = 22
Elevator 2: Floor 3, IDLE, 0 requests → score = 4 + 0 = 4
Select: Elevator 2 (idle and nearby)
```

---

### 6. ElevatorController (Singleton Pattern)

**Design Pattern**: Singleton Pattern

**Why Singleton?**
1. **Single Point of Control**: Only one controller manages system
2. **Global Access**: Any part of system can access controller
3. **Consistent State**: No conflicting controllers
4. **Resource Management**: Manages shared resources (elevators)

**Implementation**:
```cpp
class ElevatorController {
private:
    static ElevatorController* instance;
    
    // Private constructor (can't create outside)
    ElevatorController(int numElevators, int minFloor, int maxFloor);
    
    // Delete copy operations
    ElevatorController(const ElevatorController&) = delete;
    ElevatorController& operator=(const ElevatorController&) = delete;

public:
    static ElevatorController* getInstance(int num = 3, int min = 0, int max = 10) {
        if (instance == nullptr) {
            instance = new ElevatorController(num, min, max);
        }
        return instance;
    }
};
```

**Controller Responsibilities**:

1. **Manage Elevators**:
```cpp
vector<unique_ptr<Elevator>> elevators;
```

2. **Queue External Requests**:
```cpp
set<ExternalRequest> externalRequests;
```

3. **Coordinate Scheduling**:
```cpp
unique_ptr<ElevatorScheduler> scheduler;
```

**Request Processing Flow**:
```
1. User presses floor button
   ↓
2. Controller.addExternalRequest(floor, direction)
   ↓
3. Request added to queue (set)
   ↓
4. Controller.processRequests()
   ↓
5. For each request:
   - Scheduler selects best elevator
   - Add floor as internal request to that elevator
   - Remove from external queue
   ↓
6. Controller.processInternalRequests()
   ↓
7. Each elevator processes its internal requests
   ↓
8. Elevators move and update status
```

---

## 🎨 Design Patterns Summary

### 1. Strategy Pattern (Scheduling)
**Where**: ElevatorScheduler hierarchy
**Benefit**: Interchangeable algorithms
**Example**: Switch from FCFS to LOOK at runtime

### 2. Singleton Pattern (Controller)
**Where**: ElevatorController
**Benefit**: Single point of control
**Example**: Only one controller manages system

### 3. State Pattern (Implicit)
**Where**: Direction enum, DoorState enum
**Benefit**: Clear state transitions
**Example**: Door: CLOSED → OPENING → OPEN

### 4. Command Pattern (Implicit)
**Where**: Request objects
**Benefit**: Requests as first-class objects
**Example**: Can queue, log, undo requests

### 5. Composite Pattern (Potential)
**Where**: Could group elevators into banks
**Benefit**: Treat individual/groups uniformly
**Example**: Express elevator bank, local bank

---

## 🛡️ SOLID Principles Application

### 1. Single Responsibility Principle (SRP)
Each class has ONE reason to change:
- `Door`: Only door operations
- `Display`: Only display logic
- `Elevator`: Only elevator state and movement
- `Scheduler`: Only scheduling logic
- `Controller`: Only system coordination

**Example**: If we change how doors work, we only modify `Door` class.

### 2. Open/Closed Principle (OCP)
Open for extension, closed for modification:
- Add new scheduler: Create new class, don't modify existing
- Add new display type: Inherit from Display, don't change Display
- Add new request type: Create new class, don't modify ExternalRequest

**Example**: Adding a new scheduler:
```cpp
class MyScheduler : public ElevatorScheduler {
    int selectElevator(...) override {
        // Your logic here
    }
};
// No existing code modified!
```

### 3. Liskov Substitution Principle (LSP)
Derived classes can replace base classes:
- Any `ElevatorScheduler` can be used by controller
- All schedulers follow same contract
- Controller doesn't care which scheduler it uses

**Example**:
```cpp
controller->setScheduler(make_unique<FCFSScheduler>());  // Works
controller->setScheduler(make_unique<SCANScheduler>());  // Works
controller->setScheduler(make_unique<LOOKScheduler>());  // Works
```

### 4. Interface Segregation Principle (ISP)
Clients depend only on what they use:
- `ElevatorScheduler` has only one method: `selectElevator()`
- Schedulers don't have unnecessary methods
- No "fat interfaces"

**Example**: Scheduler doesn't need methods like `addElevator()`, `removeElevator()`, etc.

### 5. Dependency Inversion Principle (DIP)
Depend on abstractions, not concretions:
- Controller depends on `ElevatorScheduler` (abstraction)
- Not on `FCFSScheduler` (concrete implementation)
- High-level doesn't depend on low-level details

**Example**:
```cpp
class ElevatorController {
private:
    unique_ptr<ElevatorScheduler> scheduler;  // Abstraction, not concrete!
};
```

---

## 🔄 Complete Flow Example

### Scenario: Person at Floor 5 Wants to Go Up to Floor 9

**Step 1: External Request**
```cpp
controller->addExternalRequest(5, Direction::UP);
```
- Validates floor (5 is valid)
- Validates direction (UP is valid at floor 5)
- Adds to external request queue

**Step 2: Process External Request**
```cpp
controller->processRequests();
```
- Gets request from queue: {floor: 5, direction: UP}
- Calls scheduler: `scheduler->selectElevator(elevators, request)`

**Step 3: Scheduler Selection (LOOK)**
```
Evaluating elevators:
- Elevator 1: Floor 3, Direction UP → Score = 2 (distance) + 10 (1 request) = 12
- Elevator 2: Floor 8, Direction DOWN → Score = 500 + 3 (opposite) = 503
- Elevator 3: Floor 5, Direction IDLE → Score = 0 (same floor) = 0

Selected: Elevator 3 (best score)
```

**Step 4: Assign to Elevator**
```cpp
elevators[2]->addInternalRequest(5);  // Go to floor 5 first
```
- Elevator 3 adds floor 5 to internal requests
- Determines direction: UP (since 5 > current)

**Step 5: Elevator Moves to Floor 5**
```cpp
elevator3->processNextRequest();
```
- Finds next floor: 5
- Moves: currentFloor++ until reaches 5
- Opens door at floor 5

**Step 6: Person Enters and Presses 9**
```cpp
elevator3->addInternalRequest(9);
```
- Adds 9 to internal requests
- Internal requests now: {9}

**Step 7: Process Internal Request**
```cpp
controller->processInternalRequests();
```
- Elevator 3 processes request for floor 9
- Moves: currentFloor++ until reaches 9
- Opens door at floor 9

**Step 8: Complete**
- Person exits
- Door closes
- Elevator becomes IDLE

---

## 🧵 Thread Safety Considerations

**Current Implementation**: Single-threaded

**For Production, Add**:

### 1. Request Queue Protection
```cpp
class ElevatorController {
private:
    std::mutex requestMutex;
    std::set<ExternalRequest> externalRequests;

public:
    bool addExternalRequest(int floor, Direction dir) {
        std::lock_guard<std::mutex> lock(requestMutex);
        externalRequests.insert({floor, dir});
    }
};
```

### 2. Elevator State Protection
```cpp
class Elevator {
private:
    std::atomic<int> currentFloor;
    std::mutex requestMutex;

public:
    bool addInternalRequest(int floor) {
        std::lock_guard<std::mutex> lock(requestMutex);
        internalRequests.insert(floor);
    }
};
```

### 3. Separate Processing Thread
```cpp
class ElevatorController {
private:
    std::thread processingThread;
    std::atomic<bool> running;

    void processLoop() {
        while (running) {
            processRequests();
            processInternalRequests();
            std::this_thread::sleep_for(100ms);
        }
    }
};
```

---

## 📈 Performance Analysis

### Time Complexity

| Operation | Complexity | Explanation |
|-----------|-----------|-------------|
| Add External Request | O(log R) | Set insertion |
| Add Internal Request | O(log F) | Set insertion |
| Select Elevator | O(E) | Iterate all elevators |
| Process Single Request | O(F) | Move floor by floor |
| Display Status | O(E) | Show all elevators |

Where:
- R = pending external requests
- F = number of floors
- E = number of elevators

### Space Complexity

| Component | Complexity | Explanation |
|-----------|-----------|-------------|
| Per Elevator | O(F) | Internal requests |
| Controller | O(E × F) | All elevators |
| External Queue | O(R) | Pending requests |
| Total System | O(E × F + R) | Combined |

### Optimization Opportunities

1. **Caching**: Cache frequently accessed floor calculations
2. **Indexing**: Index elevators by floor for faster lookup
3. **Batching**: Process multiple requests in parallel
4. **Prediction**: Use ML to predict request patterns

---

## 🔮 Future Enhancements

### 1. Advanced Features
```cpp
class Elevator {
private:
    int capacity;           // Maximum passengers
    int currentWeight;      // Current load
    bool maintenanceMode;   // Out of service
    int energyConsumption;  // Power usage tracking
};
```

### 2. Smart Scheduling
```cpp
class MLScheduler : public ElevatorScheduler {
private:
    NeuralNetwork model;  // Trained on usage patterns
    
public:
    int selectElevator(...) override {
        // Predict optimal elevator using ML
        auto prediction = model.predict(request);
        return findBestMatch(prediction);
    }
};
```

### 3. Emergency Handling
```cpp
enum class EmergencyType {
    FIRE, POWER_FAILURE, STUCK, OVERLOAD
};

class Elevator {
public:
    void handleEmergency(EmergencyType type) {
        switch (type) {
            case FIRE:
                goToGroundFloor();
                door->emergencyOpen();
                break;
            case POWER_FAILURE:
                useBackupPower();
                break;
            // ...
        }
    }
};
```

### 4. Access Control
```cpp
struct SecureRequest {
    int floor;
    Direction direction;
    string accessCard;  // Security badge
    SecurityLevel level;  // Clearance level
};

class SecureElevator : public Elevator {
public:
    bool validateAccess(const SecureRequest& req) {
        return securitySystem.checkClearance(req.accessCard, req.floor);
    }
};
```

---

## 🎓 Key Takeaways

### Design Principles
1. ✅ **Separation of Concerns**: Each class has distinct responsibility
2. ✅ **Encapsulation**: Internal state is protected
3. ✅ **Composition over Inheritance**: Elevator HAS-A Door, not IS-A Door
4. ✅ **Dependency Injection**: Controller receives scheduler
5. ✅ **Interface Programming**: Program to abstractions

### Best Practices
1. ✅ **Type Safety**: Enum classes instead of integers
2. ✅ **Smart Pointers**: unique_ptr for ownership
3. ✅ **Const Correctness**: Const methods where appropriate
4. ✅ **Clear Naming**: Self-documenting code
5. ✅ **Comprehensive Comments**: Explain "why", not just "what"

### Scalability
1. ✅ **Easy to extend**: New schedulers, new features
2. ✅ **Easy to test**: Mock dependencies
3. ✅ **Easy to maintain**: Clear structure
4. ✅ **Easy to understand**: Well-documented

---

## 📚 Learning Resources

### Related Concepts
- **Scheduling Algorithms**: OS scheduling, disk scheduling
- **State Machines**: Finite state automata
- **Concurrency**: Mutexes, semaphores, atomic operations
- **Real-Time Systems**: Hard vs soft real-time constraints

### Interview Focus
- Explain design patterns used
- Justify design decisions
- Discuss trade-offs
- Handle edge cases
- Scale to larger systems

---

## ✨ Conclusion

This elevator system demonstrates:
- **Clean Architecture**: Well-structured, maintainable code
- **Design Patterns**: Practical application of patterns
- **SOLID Principles**: All five principles applied
- **Extensibility**: Easy to add features
- **Production-Ready Foundation**: Can be extended for real use

The design balances **simplicity** with **functionality**, providing a solid foundation that can be extended for production use while remaining clear and educational.
