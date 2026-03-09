# Elevator System - All Design Choices Explained

## 🎯 Purpose of This Document
This document explains **every significant design choice** made in the elevator system, the **alternatives considered**, and **why specific decisions were made**.

---

## 📋 Table of Contents
1. [Architectural Decisions](#architectural-decisions)
2. [Data Structure Choices](#data-structure-choices)
3. [Design Pattern Selections](#design-pattern-selections)
4. [Class Design Decisions](#class-design-decisions)
5. [Algorithm Choices](#algorithm-choices)
6. [Type Safety Decisions](#type-safety-decisions)
7. [Memory Management](#memory-management)
8. [Extensibility Considerations](#extensibility-considerations)

---

## 🏛️ Architectural Decisions

### Decision 1: Multi-Class Architecture vs Single Class

**Chosen**: Multi-class with clear separation of concerns

**Alternatives Considered**:
1. **Single monolithic Elevator class** with everything
2. **Two classes**: Elevator + Controller
3. **Multi-class** (chosen)

**Why Chosen**:
```
Single Class:
❌ Violates Single Responsibility Principle
❌ Hard to test individual components
❌ Difficult to maintain
❌ Poor reusability

Two Classes:
⚠️ Still mixes door, display logic with elevator
⚠️ Scheduling logic embedded in controller

Multi-Class:
✅ Clear separation of concerns
✅ Easy to test each component
✅ Reusable components
✅ Follows SOLID principles
✅ Each class has one reason to change
```

**Impact**:
- More files to manage
- Slightly more complex initially
- Much easier to maintain long-term
- Better for interviews (shows design thinking)

---

### Decision 2: Composition vs Inheritance for Door/Display

**Chosen**: Composition (HAS-A relationship)

**Code**:
```cpp
class Elevator {
private:
    std::unique_ptr<Door> door;         // HAS-A Door
    std::unique_ptr<Display> display;   // HAS-A Display
};
```

**Alternatives Considered**:
1. **Inheritance**: `class Elevator : public Door, public Display`
2. **Direct implementation**: Door logic inside Elevator
3. **Composition** (chosen)

**Why Composition**:
```
Inheritance:
❌ Elevator IS-NOT-A Door
❌ Creates tight coupling
❌ Can't change door implementation easily
❌ Multiple inheritance is complex

Direct Implementation:
❌ Violates Single Responsibility
❌ Makes Elevator class too large
❌ Can't reuse door logic

Composition:
✅ Elevator HAS-A Door (correct relationship)
✅ Loose coupling
✅ Can swap implementations
✅ Can reuse Door in other contexts
✅ "Favor composition over inheritance"
```

**Real-World Analogy**:
- A car HAS-A engine (composition) ✅
- A car IS-NOT-A engine (inheritance) ❌

---

### Decision 3: Centralized Controller vs Distributed Control

**Chosen**: Centralized ElevatorController (Singleton)

**Alternatives**:
1. **Distributed**: Each elevator makes own decisions
2. **Peer-to-peer**: Elevators communicate directly
3. **Centralized** (chosen)

**Why Centralized**:
```
Distributed:
❌ Coordination difficult
❌ Conflicting decisions possible
❌ Hard to implement global optimization
❌ Complex message passing

Peer-to-Peer:
❌ Network complexity
❌ Consensus problems
❌ Difficult to debug

Centralized:
✅ Single source of truth
✅ Global optimization possible
✅ Easier to debug
✅ Consistent state
✅ Matches real elevator systems
```

**Real Systems**: Almost all elevator systems use centralized control.

---

## 📊 Data Structure Choices

### Decision 4: std::set vs std::vector for Internal Requests

**Chosen**: `std::set<int>` for internal requests

**Code**:
```cpp
class Elevator {
private:
    std::set<int> internalRequests;  // Chosen
};
```

**Alternatives**:
| Data Structure | Pros | Cons | Chosen? |
|----------------|------|------|---------|
| `vector<int>` | Fast iteration | Need manual sorting, duplicates | ❌ |
| `priority_queue<int>` | Auto-sorted | Can't iterate easily, duplicates | ❌ |
| `set<int>` | Auto-sorted, no duplicates, iteratable | Slightly slower | ✅ |
| `unordered_set<int>` | Fast lookup | Not sorted | ❌ |

**Why std::set**:
```cpp
// Example scenario
internalRequests.insert(8);
internalRequests.insert(3);
internalRequests.insert(10);
internalRequests.insert(3);  // Duplicate ignored

// Result: {3, 8, 10}  (automatically sorted!)

// Find next floor above 5
auto it = internalRequests.upper_bound(5);
// Returns iterator to 8

// Find next floor below 5
auto it = internalRequests.lower_bound(5);
--it;
// Returns iterator to 3
```

**Complexity**:
- Insert: O(log n)
- Find next: O(log n)
- Remove: O(log n)

With vector:
- Insert sorted: O(n)
- Find next: O(n)
- Remove: O(n)

**Decision**: O(log n) is much better than O(n) for typical operations.

---

### Decision 5: std::set vs std::queue for External Requests

**Chosen**: `std::set<ExternalRequest>`

**Alternatives**:
| Data Structure | Why Considered | Why Not Chosen |
|----------------|----------------|----------------|
| `queue<ExternalRequest>` | FIFO order | Can't prevent duplicates, no prioritization | ❌ |
| `priority_queue<ExternalRequest>` | Auto-prioritized | Can't iterate, can't remove specific | ❌ |
| `vector<ExternalRequest>` | Flexible | Manual management, duplicates | ❌ |
| `set<ExternalRequest>` | No duplicates, ordered | Requires comparison operator | ✅ |

**Implementation**:
```cpp
struct ExternalRequest {
    int floor;
    Direction direction;
    
    bool operator<(const ExternalRequest& other) const {
        if (floor != other.floor) return floor < other.floor;
        return direction < other.direction;
    }
};

// Usage
std::set<ExternalRequest> externalRequests;
externalRequests.insert({5, Direction::UP});
externalRequests.insert({5, Direction::UP});  // Duplicate ignored!
```

**Benefits**:
- No duplicate requests
- Ordered by floor (easy to process)
- Easy to iterate
- Easy to remove specific request

---

## 🎨 Design Pattern Selections

### Decision 6: Strategy Pattern for Scheduling

**Chosen**: Strategy Pattern

**Structure**:
```cpp
// Strategy Interface
class ElevatorScheduler {
public:
    virtual int selectElevator(...) = 0;
};

// Concrete Strategies
class FCFSScheduler : public ElevatorScheduler { };
class SCANScheduler : public ElevatorScheduler { };
class LOOKScheduler : public ElevatorScheduler { };

// Context
class ElevatorController {
private:
    std::unique_ptr<ElevatorScheduler> scheduler;
};
```

**Alternatives**:
1. **Hard-coded** scheduling in controller
2. **Switch statement** to select algorithm
3. **Strategy Pattern** (chosen)

**Comparison**:
```cpp
// Alternative 1: Hard-coded (Bad)
class ElevatorController {
    int selectElevator(...) {
        // FCFS logic hard-coded here
        // Cannot change without modifying class
    }
};

// Alternative 2: Switch (Better, but not great)
class ElevatorController {
    enum SchedulingType { FCFS, SCAN, LOOK };
    SchedulingType type;
    
    int selectElevator(...) {
        switch(type) {
            case FCFS: /* logic */ break;
            case SCAN: /* logic */ break;
            case LOOK: /* logic */ break;
        }
        // Violates Open/Closed Principle
    }
};

// Chosen: Strategy Pattern (Best)
class ElevatorController {
    std::unique_ptr<ElevatorScheduler> scheduler;
    
    int selectElevator(...) {
        return scheduler->selectElevator(...);
        // Open/Closed Principle satisfied!
    }
};
```

**Why Strategy Pattern**:
✅ **Open/Closed**: Add new algorithms without modifying existing code
✅ **Runtime switching**: Change algorithm on-the-fly
✅ **Testing**: Test each algorithm independently
✅ **Clarity**: Each algorithm in its own class
✅ **Comparison**: Easy to compare performance

**Interview Gold**: This is a textbook use of Strategy Pattern!

---

### Decision 7: Singleton Pattern for Controller

**Chosen**: Singleton Pattern for ElevatorController

**Implementation**:
```cpp
class ElevatorController {
private:
    static ElevatorController* instance;
    
    // Private constructor
    ElevatorController(int num, int min, int max);
    
    // Delete copy operations
    ElevatorController(const ElevatorController&) = delete;
    ElevatorController& operator=(const ElevatorController&) = delete;

public:
    static ElevatorController* getInstance(...) {
        if (instance == nullptr) {
            instance = new ElevatorController(...);
        }
        return instance;
    }
};
```

**Alternatives**:
1. **Global variable**: `ElevatorController globalController;`
2. **Pass around**: Pass controller to every function
3. **Multiple instances**: Allow multiple controllers
4. **Singleton** (chosen)

**Why Singleton**:
```
Global Variable:
❌ No encapsulation
❌ Initialization order issues
❌ Can be modified anywhere

Pass Around:
❌ Verbose
❌ Every function needs parameter
❌ Tedious

Multiple Instances:
❌ Conflicting decisions
❌ Inconsistent state
❌ Resource waste

Singleton:
✅ Controlled access
✅ Guaranteed single instance
✅ Global access point
✅ Lazy initialization
✅ Matches real systems
```

**Controversy**: Singletons are sometimes criticized as "global state in disguise."

**Our Defense**:
- Elevator system **should** have single controller
- Matches real-world architecture
- No need for multiple instances
- Benefits outweigh drawbacks in this case

---

### Decision 8: State Pattern (Implicit) for Door

**Chosen**: Implicit State Pattern using enum

**Implementation**:
```cpp
enum class DoorState {
    OPEN, CLOSED, OPENING, CLOSING
};

class Door {
private:
    DoorState state;
    
public:
    void open() {
        if (state == CLOSED) {
            state = OPENING;
            // Simulate opening
            state = OPEN;
        }
    }
};
```

**Alternatives**:
1. **Boolean flag**: `bool isOpen;`
2. **Integer code**: `int state; // 0=closed, 1=open`
3. **Enum** (chosen)
4. **Full State Pattern**: Separate classes for each state

**Comparison**:
```cpp
// Alternative 1: Boolean (Too Simple)
bool isDoorOpen;
// Problem: Can't distinguish OPENING vs OPEN

// Alternative 2: Integer (Error-Prone)
int doorState;  // 0=closed, 1=opening, 2=open, 3=closing
// Problem: No type safety, magic numbers

// Chosen: Enum (Good Balance)
enum class DoorState { OPEN, CLOSED, OPENING, CLOSING };
// Benefits: Type-safe, clear intent, all states

// Alternative 4: Full State Pattern (Over-Engineering for this case)
class DoorState { virtual void open() = 0; };
class ClosedState : public DoorState { };
class OpenState : public DoorState { };
// Problem: Overkill for simple state machine
```

**Why Enum**:
✅ Type-safe
✅ Clear and readable
✅ All states explicit
✅ Not over-engineered
✅ Easy to extend

**When to use full State Pattern**: When state transitions are complex and involve significant behavior changes.

---

## 🧱 Class Design Decisions

### Decision 9: Separate Door Class vs Inline Logic

**Chosen**: Separate Door class

**Alternatives**:
```cpp
// Alternative 1: Inline (Not Chosen)
class Elevator {
private:
    DoorState doorState;
    
public:
    void openDoor() {
        doorState = OPENING;
        sleep(500ms);
        doorState = OPEN;
    }
    
    void closeDoor() {
        doorState = CLOSING;
        sleep(500ms);
        doorState = CLOSED;
    }
};

// Chosen: Separate Class
class Door {
private:
    DoorState state;
public:
    void open();
    void close();
};

class Elevator {
private:
    std::unique_ptr<Door> door;
public:
    void openDoor() { door->open(); }
    void closeDoor() { door->close(); }
};
```

**Why Separate Class**:

**Single Responsibility**:
- Door has one job: manage door state
- Elevator has one job: manage elevator movement
- Mixing them violates SRP

**Extensibility**:
```cpp
// Easy to extend Door without touching Elevator
class Door {
    void detectObstruction();
    void handleEmergency();
    void addSensor(Sensor* sensor);
    // Elevator class unchanged!
};
```

**Reusability**:
```cpp
// Can reuse Door in other contexts
class TrainDoor : public Door { };
class GarageDoor : public Door { };
```

**Testing**:
```cpp
// Can test door independently
TEST(DoorTest, OpenCloseTransitions) {
    Door door;
    EXPECT_EQ(door.getState(), DoorState::CLOSED);
    door.open();
    EXPECT_EQ(door.getState(), DoorState::OPEN);
}
```

---

### Decision 10: Separate Display Class vs Direct Output

**Chosen**: Separate Display class

**Why Not Direct Output**:
```cpp
// Bad: Direct output in Elevator
class Elevator {
public:
    void showStatus() {
        std::cout << "Floor: " << currentFloor << std::endl;
        // Problem: Tightly coupled to console output
    }
};
```

**Problems**:
- Hard to test (can't capture output easily)
- Can't change output format
- Can't output to different destinations
- Violates Single Responsibility

**Better: Separate Display**:
```cpp
class Display {
public:
    void showStatus(int floor, Direction dir, DoorState state) {
        // Can format however we want
        // Can output to different destinations
    }
};
```

**Benefits**:
✅ **Testing**: Mock display for testing
✅ **Flexibility**: Easy to change output format
✅ **Multiple outputs**: Console, GUI, web, log file
✅ **SRP**: Display handles presentation, Elevator handles logic

**Production Use**:
```cpp
class ConsoleDisplay : public Display { };
class LEDDisplay : public Display { };
class WebDisplay : public Display { };
class LogFileDisplay : public Display { };
```

---

### Decision 11: ExternalRequest as Struct vs Class

**Chosen**: Struct (simple data holder)

**Code**:
```cpp
struct ExternalRequest {
    int floor;
    Direction direction;
    
    ExternalRequest(int f, Direction d) : floor(f), direction(d) {}
    
    bool operator<(const ExternalRequest& other) const {
        if (floor != other.floor) return floor < other.floor;
        return direction < other.direction;
    }
};
```

**Alternatives**:
1. **Struct** (chosen)
2. **Class with encapsulation**

**Why Struct**:
```cpp
// Class alternative
class ExternalRequest {
private:
    int floor;
    Direction direction;
    
public:
    ExternalRequest(int f, Direction d) : floor(f), direction(d) {}
    int getFloor() const { return floor; }
    Direction getDirection() const { return direction; }
    void setFloor(int f) { floor = f; }  // Do we need this?
    void setDirection(Direction d) { direction = d; }  // Do we need this?
};
```

**Decision Factors**:
- **No behavior**: Request is just data, no complex logic
- **No invariants**: No constraints to maintain
- **No encapsulation needed**: Values don't need protection
- **Simplicity**: Struct is simpler and clearer

**When to use Class**:
- When you need to maintain invariants
- When you need complex behavior
- When you need encapsulation

**Our case**: Simple data holder → Struct is appropriate

---

## 🧮 Algorithm Choices

### Decision 12: SCAN Algorithm for Elevator Movement

**Chosen**: SCAN-like algorithm

**How it Works**:
```cpp
bool processNextRequest() {
    if (direction == UP) {
        // Find next floor above current
        auto it = internalRequests.upper_bound(currentFloor);
        if (it != internalRequests.end()) {
            moveToFloor(*it);  // Continue UP
        } else {
            // No more floors above, switch to DOWN
            direction = DOWN;
        }
    }
    // Similar for DOWN
}
```

**Alternatives**:
1. **FCFS**: Process requests in order received
2. **SSTF**: Always go to nearest floor
3. **SCAN** (chosen)
4. **LOOK**: SCAN but don't go to extremes

**Comparison**:
```
Request sequence: 2, 8, 4, 9, 3
Current floor: 5

FCFS: 5 → 2 → 8 → 4 → 9 → 3
Distance: 3 + 6 + 4 + 5 + 6 = 24 floors

SSTF: 5 → 4 → 3 → 2 → 8 → 9
Distance: 1 + 1 + 1 + 6 + 1 = 10 floors
Problem: Can starve far requests

SCAN: 5 → 8 → 9 → 4 → 3 → 2
Distance: 3 + 1 + 5 + 1 + 1 = 11 floors
Benefit: No starvation, predictable

LOOK: 5 → 8 → 9 → 4 → 3 → 2
Distance: Same as SCAN
Benefit: Doesn't go to extreme if no requests
```

**Why SCAN**:
✅ **Fair**: All requests eventually served
✅ **Predictable**: Users know elevator will come
✅ **Efficient**: Reasonable total distance
✅ **Standard**: Used in real elevators
❌ Slightly longer distance than SSTF

**Real World**: Most modern elevators use variations of SCAN.

---

### Decision 13: Three Scheduling Algorithms (FCFS, SCAN, LOOK)

**Why Multiple**:

**Educational Value**:
- Shows Strategy Pattern in action
- Demonstrates algorithm trade-offs
- Good interview discussion point

**Different Use Cases**:
- **FCFS**: Low traffic, fairness priority
- **SCAN**: Medium traffic, balanced
- **LOOK**: High traffic, efficiency priority

**Comparison**:
```cpp
// Scenario: Request at floor 7 going UP
// Elevator 1: Floor 3, IDLE
// Elevator 2: Floor 5, UP
// Elevator 3: Floor 9, DOWN

FCFS:
- Selects: Elevator 1 (first idle)
- Logic: Simple, first available

SCAN:
- Selects: Elevator 2 (moving toward request)
- Logic: Prefers same direction

LOOK:
- Selects: Elevator 2 (best score: 2 + penalties)
- Logic: Sophisticated scoring considering multiple factors
```

**Why LOOK as Default**:
- Most sophisticated
- Best for typical scenarios
- Shows advanced thinking

---

## 🔒 Type Safety Decisions

### Decision 14: enum class vs enum

**Chosen**: `enum class` (scoped enums)

**Code**:
```cpp
// Chosen
enum class Direction {
    UP, DOWN, IDLE
};

// Usage
Direction dir = Direction::UP;
```

**Alternatives**:
```cpp
// Alternative: Regular enum
enum Direction {
    UP, DOWN, IDLE
};

// Usage
Direction dir = UP;  // No scope
```

**Why enum class**:
```cpp
// Regular enum problems
enum Direction { UP, DOWN };
enum DoorCommand { UP, DOWN };  // ERROR: Redefinition!

int x = UP;  // Implicit conversion to int (dangerous!)

// enum class advantages
enum class Direction { UP, DOWN };
enum class DoorCommand { UP, DOWN };  // OK: Different scopes

Direction dir = Direction::UP;
int x = dir;  // ERROR: No implicit conversion (safe!)
```

**Benefits**:
✅ **Type safety**: No implicit conversions
✅ **Scope**: No name conflicts
✅ **Clarity**: Intent is clear
✅ **Modern C++**: Best practice since C++11

---

## 💾 Memory Management

### Decision 15: Smart Pointers vs Raw Pointers

**Chosen**: `std::unique_ptr` for ownership

**Code**:
```cpp
class Elevator {
private:
    std::unique_ptr<Door> door;         // Owns door
    std::unique_ptr<Display> display;   // Owns display
};

class ElevatorController {
private:
    std::vector<std::unique_ptr<Elevator>> elevators;  // Owns elevators
    std::unique_ptr<ElevatorScheduler> scheduler;     // Owns scheduler
};
```

**Alternatives**:
1. **Raw pointers**: `Door* door;`
2. **Shared pointers**: `std::shared_ptr<Door> door;`
3. **Unique pointers** (chosen)

**Comparison**:
```cpp
// Raw Pointers (C-style, not chosen)
class Elevator {
private:
    Door* door;
public:
    Elevator() {
        door = new Door();  // Manual allocation
    }
    ~Elevator() {
        delete door;  // Manual deletion - easy to forget!
    }
};
// Problems: Memory leaks, manual management, double-delete bugs

// Shared Pointers (Over-engineered for this case)
class Elevator {
private:
    std::shared_ptr<Door> door;
public:
    Elevator() {
        door = std::make_shared<Door>();
    }
    // Automatic cleanup, but unnecessary overhead
};
// Problem: Shared ownership not needed here

// Unique Pointers (Chosen)
class Elevator {
private:
    std::unique_ptr<Door> door;
public:
    Elevator() {
        door = std::make_unique<Door>();
    }
    // Automatic cleanup, clear ownership, no overhead
};
```

**Why unique_ptr**:
✅ **Automatic cleanup**: No memory leaks
✅ **Clear ownership**: Elevator owns door
✅ **Move semantics**: Efficient transfers
✅ **No overhead**: Same as raw pointer
✅ **Modern C++**: Best practice

**When to use shared_ptr**: When multiple objects need to share ownership.

**Our case**: Clear single ownership → unique_ptr is perfect

---

### Decision 16: Value Semantics vs Pointer Semantics for Elevators

**Chosen**: Store elevators by unique_ptr in vector

**Code**:
```cpp
std::vector<std::unique_ptr<Elevator>> elevators;
```

**Alternatives**:
```cpp
// Alternative 1: Store by value
std::vector<Elevator> elevators;

// Alternative 2: Store raw pointers
std::vector<Elevator*> elevators;

// Chosen: Store unique_ptr
std::vector<std::unique_ptr<Elevator>> elevators;
```

**Why unique_ptr in vector**:
```cpp
// By value problems
std::vector<Elevator> elevators;
elevators.push_back(elevator);  // Copies entire elevator!
// If vector resizes, all elevators copied again!
// Large objects, expensive copies

// Raw pointer problems
std::vector<Elevator*> elevators;
elevators.push_back(new Elevator(...));
// Manual deletion needed
// Easy to leak memory

// unique_ptr solution
std::vector<std::unique_ptr<Elevator>> elevators;
elevators.push_back(std::make_unique<Elevator>(...));
// No copies, automatic cleanup, clear ownership
```

**Benefits**:
✅ **No copies**: Elevators stay in place
✅ **Automatic cleanup**: No leaks
✅ **Stable addresses**: Elevator pointers don't invalidate
✅ **Clear ownership**: Vector owns elevators

---

## 🔧 Extensibility Considerations

### Decision 17: Scheduler as Abstract Base Class

**Chosen**: Abstract base class for flexibility

**Structure**:
```cpp
class ElevatorScheduler {
public:
    virtual ~ElevatorScheduler() = default;
    virtual int selectElevator(...) = 0;  // Pure virtual
    virtual std::string getName() const = 0;  // Pure virtual
};
```

**Why Abstract Base Class**:
✅ **Polymorphism**: Different implementations
✅ **Interface contract**: All schedulers follow same API
✅ **Easy extension**: Just inherit and implement
✅ **No partial implementation**: Can't instantiate base class

**Adding New Scheduler**:
```cpp
// Step 1: Create new class
class MyScheduler : public ElevatorScheduler {
public:
    int selectElevator(...) override {
        // Your logic here
    }
    
    std::string getName() const override {
        return "My Custom Scheduler";
    }
};

// Step 2: Use it
controller->setScheduler(std::make_unique<MyScheduler>());

// That's it! No modification to existing code!
```

**No Changes Needed**:
- ElevatorController: Same
- Other schedulers: Same
- Elevator: Same
- Main: Just one line to switch

**This is Open/Closed Principle in action!**

---

### Decision 18: Const Correctness Throughout

**Chosen**: Use `const` extensively

**Examples**:
```cpp
class Elevator {
public:
    int getId() const { return id; }  // Doesn't modify object
    int getCurrentFloor() const { return currentFloor; }
    bool isIdle() const { return direction == Direction::IDLE; }
    
    void showStatus() const;  // Read-only operation
};

class Display {
public:
    void showStatus(int floor, Direction dir, DoorState state) const;
    // Doesn't modify Display, only shows information
};

class ElevatorController {
public:
    void displayStatus() const;  // Read-only
    int getNumElevators() const { return elevators.size(); }
};
```

**Benefits**:
✅ **Compiler enforcement**: Catches accidental modifications
✅ **Interface clarity**: Shows read-only vs modifying operations
✅ **Optimization**: Compiler can optimize const functions
✅ **Thread safety**: Const operations are thread-safe (reading)

**Rule**: If a method doesn't modify the object, make it `const`.

---

## 📊 Summary Table: All Major Decisions

| Decision | Chosen | Alternative | Reason |
|----------|---------|-------------|---------|
| Architecture | Multi-class | Single class | Separation of concerns |
| Door/Display | Composition | Inheritance | HAS-A not IS-A |
| Controller | Centralized (Singleton) | Distributed | Single source of truth |
| Internal requests | `std::set` | `std::vector` | Auto-sorted, no duplicates |
| External requests | `std::set` | `std::queue` | No duplicates, ordered |
| Scheduling | Strategy Pattern | Hard-coded | Extensibility, OCP |
| Controller instance | Singleton | Multiple instances | Single control point |
| Door states | Enum class | Boolean/integer | Type safety, clarity |
| Door class | Separate class | Inline logic | SRP, reusability |
| Display class | Separate class | Direct output | Testability, flexibility |
| ExternalRequest | Struct | Class | Simple data holder |
| Movement algorithm | SCAN | FCFS/SSTF | Fair, predictable |
| Schedulers | Three (FCFS/SCAN/LOOK) | One | Show trade-offs |
| Direction enum | enum class | enum | Type safety |
| Pointers | `unique_ptr` | Raw pointers | Memory safety |
| Elevator storage | `vector<unique_ptr>` | `vector<Elevator>` | No copies |
| Scheduler base | Abstract base | Concrete | Polymorphism |
| Methods | Const where appropriate | All non-const | Safety, clarity |

---

## 🎓 Interview Tips

### Questions You Should Be Able To Answer

1. **Why Strategy Pattern for schedulers?**
   - Answer: Allows runtime algorithm switching, satisfies Open/Closed Principle, easy to add new algorithms

2. **Why Singleton for Controller?**
   - Answer: Ensures single point of control, matches real-world architecture, prevents state conflicts

3. **Why separate Door class?**
   - Answer: Single Responsibility Principle, extensibility, reusability, testability

4. **Why std::set for requests?**
   - Answer: Automatic sorting, no duplicates, O(log n) operations

5. **Why SCAN algorithm?**
   - Answer: Fair, predictable, efficient, standard in real elevators

6. **Why enum class over enum?**
   - Answer: Type safety, scope, no implicit conversions, modern C++

7. **Why unique_ptr over raw pointers?**
   - Answer: Automatic memory management, clear ownership, no leaks

---

## 🏆 What Makes This Design Good

### For Interviews
✅ Shows deep understanding of design patterns
✅ Demonstrates SOLID principles
✅ Clear reasoning for every choice
✅ Considers trade-offs
✅ Extensible and maintainable

### For Production
✅ Clean architecture
✅ Easy to test
✅ Easy to extend
✅ Well-documented
✅ Memory-safe
✅ Type-safe

### For Learning
✅ Comprehensive examples
✅ Real-world applicable
✅ Multiple patterns shown
✅ Best practices demonstrated
✅ Commented and explained

---

## 🎯 Conclusion

Every decision in this system was made deliberately with clear reasoning:
- **Architectural**: Clean separation, clear responsibilities
- **Patterns**: Appropriate use of proven patterns
- **Safety**: Type-safe, memory-safe
- **Extensibility**: Easy to modify and extend
- **Maintainability**: Clear, documented, testable

This is not just code that works—it's code that demonstrates **professional software engineering** principles.
