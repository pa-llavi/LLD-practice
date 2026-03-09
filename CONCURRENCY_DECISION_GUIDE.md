# When to Use Mutex vs Atomic vs Locks - Complete Guide

## 🎯 TL;DR - Quick Decision Tree

```
┌─────────────────────────────────────────┐
│ What are you protecting?                │
└──────────────┬──────────────────────────┘
               │
       ┌───────┴────────┐
       │                │
   Simple type?    Complex data?
   (int, bool)     (container, object)
       │                │
       ▼                ▼
   Is it just      Need to protect
   read/write?     multiple operations?
       │                │
       ▼                ▼
   ATOMIC          MUTEX + LOCK
```

---

## 📚 The Three Main Tools

### 1. `std::atomic<T>` - Lock-Free Operations
### 2. `std::mutex` - Mutual Exclusion Lock
### 3. Locking Mechanisms - RAII Wrappers

---

## 🔬 Detailed Comparison

| Feature | `std::atomic` | `std::mutex` |
|---------|---------------|--------------|
| **Use for** | Simple types (int, bool, pointer) | Complex data (containers, objects) |
| **Protection** | Single variable | Critical section (multiple lines) |
| **Performance** | Fastest (lock-free) | Slower (blocking) |
| **Complexity** | Simple | Can be complex |
| **Granularity** | Single operation | Multiple operations |
| **Blocking** | Never blocks | Can block threads |
| **Best for** | Flags, counters, state | Collections, multi-step operations |

---

## 🎯 Decision Guide: Atomic vs Mutex

### Use `std::atomic` When:

```cpp
✅ Single simple variable (int, bool, pointer, enum)
✅ Read/write operations only
✅ No need to protect multiple operations together
✅ Maximum performance needed
✅ Lock-free guarantee important
```

**Examples:**
```cpp
// ✅ GOOD: Use atomic
std::atomic<int> currentFloor;
std::atomic<bool> isRunning;
std::atomic<Direction> direction;
std::atomic<int*> headPointer;

// Simple operations
currentFloor.store(5, std::memory_order_release);
int floor = currentFloor.load(std::memory_order_acquire);
currentFloor++;  // Atomic increment
```

### Use `std::mutex` When:

```cpp
✅ Protecting containers (vector, set, map, etc.)
✅ Protecting multiple related variables
✅ Multiple operations must be atomic together
✅ Protecting complex objects
✅ Need to wait/block on condition
```

**Examples:**
```cpp
// ✅ GOOD: Use mutex
std::mutex requestMutex;
std::set<int> internalRequests;  // Complex container

{
    std::lock_guard<std::mutex> lock(requestMutex);
    internalRequests.insert(5);   // Modify container safely
    internalRequests.erase(3);    // Multiple operations
}
```

---

## 📊 Real Examples from Elevator System

### Example 1: Current Floor (Use Atomic)

```cpp
// ❌ WITHOUT synchronization (WRONG)
class Elevator {
private:
    int currentFloor;  // ⚠️ Race condition!
    
public:
    void moveUp() { currentFloor++; }  // ⚠️ Not atomic!
    int getFloor() { return currentFloor; }  // ⚠️ Can read torn value!
};

// ✅ WITH atomic (RIGHT - if only reading/writing)
class Elevator {
private:
    std::atomic<int> currentFloor;
    
public:
    void moveUp() { 
        currentFloor.fetch_add(1, std::memory_order_relaxed);
    }
    
    int getFloor() { 
        return currentFloor.load(std::memory_order_acquire);
    }
};

// ✅ ALSO WORKS: Mutex (but overkill)
class Elevator {
private:
    int currentFloor;
    mutable std::mutex floorMutex;
    
public:
    void moveUp() {
        std::lock_guard<std::mutex> lock(floorMutex);
        currentFloor++;
    }
    
    int getFloor() {
        std::lock_guard<std::mutex> lock(floorMutex);
        return currentFloor;
    }
};
```

**Decision: Use `atomic<int>` - simpler, faster, sufficient**

---

### Example 2: Request Queue (Use Mutex)

```cpp
// ❌ WRONG: Can't use atomic for containers
std::atomic<std::set<int>> requests;  // ⚠️ Won't compile!

// ✅ RIGHT: Use mutex
class Elevator {
private:
    std::set<int> internalRequests;
    mutable std::mutex requestMutex;
    
public:
    void addRequest(int floor) {
        std::lock_guard<std::mutex> lock(requestMutex);
        internalRequests.insert(floor);  // Modify container
    }
    
    bool hasRequests() const {
        std::lock_guard<std::mutex> lock(requestMutex);
        return !internalRequests.empty();  // Read container
    }
    
    std::set<int> getRequestsCopy() const {
        std::lock_guard<std::mutex> lock(requestMutex);
        return internalRequests;  // Return copy
    }
};
```

**Decision: Use `mutex` - protecting complex container**

---

### Example 3: Multiple Related Variables (Use Mutex)

```cpp
// ❌ WRONG: Separate atomics don't guarantee consistency
class Elevator {
private:
    std::atomic<int> currentFloor;
    std::atomic<Direction> direction;
    
public:
    void moveUp() {
        currentFloor++;        // Operation 1
        direction = UP;        // Operation 2
        // ⚠️ Another thread might see floor++ but direction still DOWN!
    }
};

// ✅ RIGHT: Use mutex to keep consistent
class Elevator {
private:
    int currentFloor;
    Direction direction;
    mutable std::mutex stateMutex;
    
public:
    void moveUp() {
        std::lock_guard<std::mutex> lock(stateMutex);
        currentFloor++;
        direction = UP;
        // ✅ Both updated atomically together
    }
    
    std::pair<int, Direction> getState() {
        std::lock_guard<std::mutex> lock(stateMutex);
        return {currentFloor, direction};
        // ✅ Consistent snapshot
    }
};
```

**Decision: Use `mutex` - need to update multiple variables atomically**

---

## 🔐 Locking Mechanisms - When to Use Each

### 1. `std::lock_guard` - Most Common (RAII)

**Use When:**
```
✅ Simple lock/unlock for entire scope
✅ Don't need early unlock
✅ Don't need to transfer ownership
✅ Most common case (90% of the time)
```

**Example:**
```cpp
void addRequest(int floor) {
    std::lock_guard<std::mutex> lock(requestMutex);
    internalRequests.insert(floor);
    // Automatically unlocks when leaving scope
}
```

**Pros:**
- ✅ Simplest and safest
- ✅ Can't forget to unlock
- ✅ Exception-safe
- ✅ Zero overhead

**Cons:**
- ❌ Can't unlock early
- ❌ Can't transfer ownership

---

### 2. `std::unique_lock` - Flexible

**Use When:**
```
✅ Need to unlock before end of scope
✅ Need deferred locking
✅ Need to transfer lock ownership
✅ Using with condition variables
```

**Example:**
```cpp
void processRequests() {
    std::unique_lock<std::mutex> lock(requestMutex);
    
    if (requests.empty()) {
        return;  // Lock still held - might be wasteful
    }
    
    auto request = requests.front();
    requests.pop();
    
    lock.unlock();  // ✅ Release lock early
    
    // Do expensive work without holding lock
    processExpensiveOperation(request);
}

// With condition variable
void waitForRequests() {
    std::unique_lock<std::mutex> lock(requestMutex);
    cv.wait(lock, [this] { return !requests.empty(); });
    // ✅ unique_lock required for wait()
}

// Deferred locking
std::unique_lock<std::mutex> lock(mutex, std::defer_lock);
// ... do some work ...
lock.lock();  // Lock when ready
```

**Pros:**
- ✅ Very flexible
- ✅ Can unlock early
- ✅ Can transfer ownership
- ✅ Works with condition variables

**Cons:**
- ⚠️ Slight overhead vs lock_guard
- ⚠️ More complex

---

### 3. `std::scoped_lock` - Multiple Mutexes (C++17)

**Use When:**
```
✅ Need to lock multiple mutexes
✅ Want to avoid deadlock
✅ C++17 or later available
```

**Example:**
```cpp
// ❌ WRONG: Can deadlock
void transfer(Elevator& from, Elevator& to, int floor) {
    std::lock_guard<std::mutex> lock1(from.mutex);
    std::lock_guard<std::mutex> lock2(to.mutex);  // ⚠️ Can deadlock!
    // If another thread locks in opposite order: DEADLOCK
}

// ✅ RIGHT: Deadlock-free
void transfer(Elevator& from, Elevator& to, int floor) {
    std::scoped_lock lock(from.mutex, to.mutex);
    // ✅ Locks both in consistent order, no deadlock
    
    from.removeRequest(floor);
    to.addRequest(floor);
}
```

**Pros:**
- ✅ Prevents deadlock automatically
- ✅ Simple syntax for multiple locks
- ✅ Exception-safe

**Cons:**
- ⚠️ Requires C++17

---

### 4. `std::shared_lock` - Reader/Writer Locks

**Use When:**
```
✅ Many readers, few writers
✅ Reading is much more common than writing
✅ Want readers to run concurrently
```

**Requires:** `std::shared_mutex` (C++17)

**Example:**
```cpp
class ElevatorStatus {
private:
    std::map<int, ElevatorInfo> elevators;
    mutable std::shared_mutex statusMutex;
    
public:
    // Multiple readers can run concurrently
    ElevatorInfo getStatus(int id) const {
        std::shared_lock<std::shared_mutex> lock(statusMutex);
        return elevators.at(id);  // ✅ Read-only, many threads OK
    }
    
    // Writer has exclusive access
    void updateStatus(int id, ElevatorInfo info) {
        std::unique_lock<std::shared_mutex> lock(statusMutex);
        elevators[id] = info;  // ✅ Exclusive write access
    }
};
```

**Pros:**
- ✅ Better performance for read-heavy workloads
- ✅ Multiple concurrent readers

**Cons:**
- ⚠️ More complex
- ⚠️ Overhead if not read-heavy
- ⚠️ Can starve writers

---

## 🎯 Decision Matrix for Elevator System

### Component-by-Component Analysis

#### 1. Current Floor

```cpp
// Data: Single integer
// Operations: Read, increment/decrement
// Access pattern: Frequent reads, moderate writes

// ✅ BEST CHOICE: atomic<int>
std::atomic<int> currentFloor;

// Reasoning:
// - Simple type (int)
// - Single variable
// - High performance needed
// - Lock-free is beneficial
```

#### 2. Direction

```cpp
// Data: Enum value
// Operations: Read, set
// Access pattern: Frequent reads

// ✅ BEST CHOICE: atomic<Direction>
std::atomic<Direction> direction;

// Reasoning:
// - Simple type (enum)
// - Single variable
// - Frequently read by scheduler
```

#### 3. Internal Requests (Set)

```cpp
// Data: std::set<int>
// Operations: Insert, erase, iterate
// Access pattern: Moderate reads and writes

// ✅ BEST CHOICE: mutex + lock_guard
std::set<int> internalRequests;
std::mutex requestMutex;

void addRequest(int floor) {
    std::lock_guard<std::mutex> lock(requestMutex);
    internalRequests.insert(floor);
}

// Reasoning:
// - Complex container
// - Multiple operations
// - Can't use atomic
// - lock_guard is simplest
```

#### 4. External Requests (Controller)

```cpp
// Data: std::set<ExternalRequest>
// Operations: Insert, erase, iterate
// Access pattern: Bursts of writes, periodic reads

// ✅ BEST CHOICE: mutex + unique_lock
std::set<ExternalRequest> externalRequests;
std::mutex requestMutex;
std::condition_variable requestCV;

void processRequests() {
    std::unique_lock<std::mutex> lock(requestMutex);
    
    while (!externalRequests.empty()) {
        auto request = *externalRequests.begin();
        externalRequests.erase(externalRequests.begin());
        
        lock.unlock();  // ✅ Release during processing
        processRequest(request);
        lock.lock();    // ✅ Relock for next iteration
    }
}

// Reasoning:
// - Complex container
// - Need early unlock for performance
// - Used with condition variable
// - unique_lock provides flexibility
```

#### 5. Singleton Instance

```cpp
// Data: Pointer to controller
// Operations: Create once, read many times
// Access pattern: One write, many reads

// ✅ BEST CHOICE: atomic + mutex (double-checked locking)
static std::atomic<ElevatorController*> instance{nullptr};
static std::mutex instanceMutex;

static ElevatorController* getInstance() {
    ElevatorController* tmp = instance.load(std::memory_order_acquire);
    if (tmp == nullptr) {  // Fast path (atomic read)
        std::lock_guard<std::mutex> lock(instanceMutex);
        tmp = instance.load(std::memory_order_relaxed);
        if (tmp == nullptr) {
            tmp = new ElevatorController();
            instance.store(tmp, std::memory_order_release);
        }
    }
    return tmp;
}

// Reasoning:
// - Atomic for fast read path
// - Mutex for safe initialization
// - Best of both worlds
```

#### 6. Elevator Collection

```cpp
// Data: std::vector<unique_ptr<Elevator>>
// Operations: Iterate, access by index
// Access pattern: Frequent reads (status), rare writes (none after init)

// ✅ BEST CHOICE: shared_mutex (if many status queries)
std::vector<std::unique_ptr<Elevator>> elevators;
mutable std::shared_mutex elevatorsMutex;

// Many concurrent readers
ElevatorInfo getElevatorStatus(int id) const {
    std::shared_lock<std::shared_mutex> lock(elevatorsMutex);
    return elevators[id]->getStatus();
}

// Exclusive writer (rare)
void addElevator(std::unique_ptr<Elevator> elevator) {
    std::unique_lock<std::shared_mutex> lock(elevatorsMutex);
    elevators.push_back(std::move(elevator));
}

// Alternative for simple case: Regular mutex
// If reads aren't much more frequent than writes
```

---

## 🧪 Common Patterns and Anti-Patterns

### ✅ Pattern 1: Minimize Critical Section

```cpp
// ❌ BAD: Long critical section
void processRequest(Request req) {
    std::lock_guard<std::mutex> lock(mutex);
    validateRequest(req);           // Fast
    updateDatabase(req);            // SLOW! 
    sendNetworkNotification(req);   // VERY SLOW!
    logToFile(req);                 // SLOW!
}

// ✅ GOOD: Minimal critical section
void processRequest(Request req) {
    // Validate first (no lock needed)
    if (!validateRequest(req)) return;
    
    {
        std::lock_guard<std::mutex> lock(mutex);
        updateLocalState(req);  // Only lock for this
    }  // Lock released
    
    // Do expensive work without lock
    updateDatabase(req);
    sendNetworkNotification(req);
    logToFile(req);
}
```

### ✅ Pattern 2: Return by Value (Safe Copy)

```cpp
// ❌ BAD: Returning reference
const std::set<int>& getRequests() const {
    std::lock_guard<std::mutex> lock(mutex);
    return internalRequests;  // ⚠️ Lock released, reference invalid!
}

// ✅ GOOD: Return copy
std::set<int> getRequests() const {
    std::lock_guard<std::mutex> lock(mutex);
    return internalRequests;  // ✅ Copy made under lock
}  // Safe to use after lock released
```

### ✅ Pattern 3: Atomic State Machine

```cpp
// For simple state machines, atomic can work
enum class State { IDLE, MOVING, DOOR_OPEN };

std::atomic<State> state{State::IDLE};

void openDoor() {
    State expected = State::IDLE;
    if (state.compare_exchange_strong(expected, State::DOOR_OPEN)) {
        // Successfully transitioned
    } else {
        // State wasn't IDLE, couldn't transition
    }
}
```

### ❌ Anti-Pattern 1: Atomic for Everything

```cpp
// ❌ BAD: Using atomics when not needed
class Data {
    std::atomic<int> value1;
    std::atomic<int> value2;
    std::atomic<int> value3;
    // If you need this many atomics, use a mutex!
};

// ✅ GOOD: One mutex for related data
class Data {
    int value1, value2, value3;
    std::mutex dataMutex;
};
```

### ❌ Anti-Pattern 2: Locking in Constructor

```cpp
// ❌ BAD: No other thread can access yet!
Elevator::Elevator() {
    std::lock_guard<std::mutex> lock(mutex);  // Unnecessary!
    currentFloor = 0;
    direction = IDLE;
}

// ✅ GOOD: No lock needed
Elevator::Elevator() {
    currentFloor = 0;
    direction = IDLE;
}
```

---

## 📊 Performance Comparison

```
Operation                  | Relative Cost
---------------------------|---------------
Atomic read/write         | 1x (baseline)
Atomic compare-exchange   | 2-3x
Uncontended mutex         | 10-20x
Contended mutex           | 100-1000x+
```

**Takeaway:** Use atomics when possible, mutexes when necessary.

---

## 🎯 Quick Reference Card

```
┌─────────────────────────────────────────────────────────┐
│               CONCURRENCY CHEAT SHEET                   │
├─────────────────────────────────────────────────────────┤
│                                                         │
│ Single int/bool/pointer?                    → atomic   │
│ Container (vector/set/map)?                 → mutex    │
│ Multiple related variables?                 → mutex    │
│ Single simple operation?                    → atomic   │
│ Multiple operations together?               → mutex    │
│                                                         │
│ Lock whole scope?                    → lock_guard      │
│ Need early unlock?                   → unique_lock     │
│ Lock multiple mutexes?               → scoped_lock     │
│ Many readers, few writers?           → shared_lock     │
│ With condition variable?             → unique_lock     │
│                                                         │
│ Read-modify-write atomically?       → compare_exchange │
│ Simple increment/decrement?          → fetch_add/sub   │
│ Just reading?                        → load()          │
│ Just writing?                        → store()         │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

---

## 🎓 Memory Ordering (Advanced)

### For Atomics: When to Use Which

```cpp
// memory_order_relaxed: No synchronization, just atomicity
counter.fetch_add(1, std::memory_order_relaxed);
// Use when: Order doesn't matter (pure counter)

// memory_order_acquire/release: Synchronization
// Release-Acquire pair forms synchronization
flag.store(true, std::memory_order_release);  // Producer
if (flag.load(std::memory_order_acquire)) {   // Consumer
    // All writes before store() visible here
}
// Use when: Need to synchronize data

// memory_order_seq_cst: Strongest (default)
counter++;  // Equivalent to memory_order_seq_cst
// Use when: Not sure, or need total order
```

**Rule of thumb:** Start with default (seq_cst), optimize later if needed.

---

## 🎯 Summary Table

| Scenario | Use This | Why |
|----------|----------|-----|
| Single int counter | `atomic<int>` | Simple, fast, lock-free |
| Single bool flag | `atomic<bool>` | Simple, fast, lock-free |
| std::vector/set/map | `mutex` + `lock_guard` | Containers need protection |
| Multiple variables together | `mutex` + `lock_guard` | Need atomic snapshot |
| Need early unlock | `mutex` + `unique_lock` | Flexibility needed |
| Condition variable | `mutex` + `unique_lock` | Required by API |
| 2+ mutexes | `scoped_lock` | Prevents deadlock |
| Read-heavy workload | `shared_mutex` + `shared_lock` | Concurrent readers |
| Singleton creation | `atomic` + `mutex` | Fast read path |

---

## 💡 Interview Tips

### What to Say:

✅ **Good:**
> "I'd use atomic<int> for currentFloor since it's a simple type with simple operations. For the request queue, I'd use a mutex with lock_guard since it's a complex container. For the singleton, I'd combine atomic for the fast read path with a mutex for safe initialization using double-checked locking."

❌ **Bad:**
> "I'd just use mutexes everywhere to be safe."

✅ **Show trade-off awareness:**
> "Atomics are faster but only work for simple types. Mutexes are more flexible but have overhead. I'd use atomics where possible for performance, mutexes where necessary for correctness."

---

## 🎯 Final Recommendation for Elevator System

```cpp
// currentFloor: atomic (simple type, frequent access)
std::atomic<int> currentFloor;

// direction: atomic (simple enum)
std::atomic<Direction> direction;

// internalRequests: mutex + lock_guard (container)
std::set<int> internalRequests;
std::mutex requestMutex;

// externalRequests: mutex + unique_lock (container + condition variable)
std::set<ExternalRequest> externalRequests;
std::mutex requestMutex;
std::condition_variable requestCV;

// Singleton: atomic + mutex (double-checked locking)
static std::atomic<Controller*> instance;
static std::mutex instanceMutex;

// Status queries: Consider shared_mutex if very frequent
std::shared_mutex statusMutex;
```

This gives you the best balance of performance, correctness, and clarity! 🚀
