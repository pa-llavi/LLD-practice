# Elevator System - UML Diagrams

## 📐 Complete UML Class Diagram

### Main Class Diagram
```
┌─────────────────────────────────────────────────────────────────────────────┐
│                         ELEVATOR SYSTEM UML                                 │
└─────────────────────────────────────────────────────────────────────────────┘

┌──────────────────────────────────┐
│     «enumeration»                │
│       Direction                  │
├──────────────────────────────────┤
│ + UP                             │
│ + DOWN                           │
│ + IDLE                           │
└──────────────────────────────────┘

┌──────────────────────────────────┐
│     «enumeration»                │
│       DoorState                  │
├──────────────────────────────────┤
│ + OPEN                           │
│ + CLOSED                         │
│ + OPENING                        │
│ + CLOSING                        │
└──────────────────────────────────┘

                    ┌──────────────────────────────────────────────┐
                    │          «singleton»                         │
                    │       ElevatorController                     │
                    ├──────────────────────────────────────────────┤
                    │ - instance: ElevatorController* {static}    │
                    │ - elevators: vector<unique_ptr<Elevator>>   │
                    │ - externalRequests: set<ExternalRequest>    │
                    │ - scheduler: unique_ptr<ElevatorScheduler>  │
                    │ - minFloor: int                             │
                    │ - maxFloor: int                             │
                    ├──────────────────────────────────────────────┤
                    │ - ElevatorController(int, int, int)         │
                    │ + getInstance(int, int, int): Controller*   │
                    │ + destroyInstance(): void {static}          │
                    │ + setScheduler(unique_ptr<Scheduler>): void │
                    │ + addExternalRequest(int, Direction): bool  │
                    │ + processRequests(): void                   │
                    │ + processInternalRequests(): void           │
                    │ + displayStatus(): void                     │
                    │ + getNumElevators(): int                    │
                    │ + getElevator(int): Elevator*               │
                    │ + getPendingRequestsCount(): int            │
                    │ - isValidFloor(int): bool                   │
                    │ - processExternalRequest(Request): bool     │
                    └──────────────┬───────────────────────────────┘
                                   │
                                   │ manages
                                   │ 1
                                   │
                    ┌──────────────┴───────────────┐
                    │                              │
                    │                              │ 1..*
                    ▼                              ▼
    ┌───────────────────────────────┐    ┌────────────────────────────────┐
    │  «interface»                  │    │         Elevator               │
    │  ElevatorScheduler            │    ├────────────────────────────────┤
    ├───────────────────────────────┤    │ - id: int                      │
    │ {abstract}                    │    │ - currentFloor: int            │
    ├───────────────────────────────┤    │ - minFloor: int                │
    │ + selectElevator(vector,      │    │ - maxFloor: int                │
    │     Request): int {abstract}  │    │ - direction: Direction         │
    │ + getName(): string {abstract}│    │ - door: unique_ptr<Door>       │
    └───────────┬───────────────────┘    │ - display: unique_ptr<Display> │
                │                         │ - internalRequests: set<int>   │
                │ implements              ├────────────────────────────────┤
       ┌────────┴────────┬───────────┐   │ + Elevator(int, int, int)      │
       │                 │           │   │ + ~Elevator()                  │
       │                 │           │   │ + getId(): int                 │
       ▼                 ▼           ▼   │ + getCurrentFloor(): int       │
┌──────────────┐  ┌─────────────┐  ┌──────────────┐
│FCFSScheduler │  │SCANScheduler│  │LOOKScheduler │  │ + getDirection(): Direction    │
├──────────────┤  ├─────────────┤  ├──────────────┤  │ + isIdle(): bool               │
│              │  │             │  │              │  │ + getInternalRequests(): set&  │
├──────────────┤  ├─────────────┤  ├──────────────┤  │ + addInternalRequest(int): bool│
│+ select...   │  │+ select...  │  │+ select...   │  │ + moveToFloor(int): void       │
│+ getName()   │  │+ getName()  │  │+ getName()   │  │ + processNextRequest(): bool   │
└──────────────┘  └─────────────┘  └──────────────┘  │ + openDoor(): void             │
                                                       │ + closeDoor(): void            │
                                                       │ + showStatus(): void           │
                                                       │ + calculateScore(int, Dir): int│
                                                       │ - determineDirection(): void   │
                                                       │ - handleDoorOperations(): void │
                                                       └────────┬───────────┬───────────┘
                                                                │           │
                                                                │ has-a     │ has-a
                                                                │ 1         │ 1
                                                                │           │
                                                                ▼           ▼
                                                       ┌────────────┐  ┌──────────┐
                                                       │    Door    │  │ Display  │
                                                       ├────────────┤  ├──────────┤
                                                       │ - state:   │  │ - id: int│
                                                       │   DoorState│  ├──────────┤
                                                       ├────────────┤  │+ Display │
                                                       │ + Door()   │  │  (int)   │
                                                       │ + getState │  │+ show    │
                                                       │   (): State│  │  Status()│
                                                       │ + isOpen() │  │+ show    │
                                                       │   : bool   │  │  Message │
                                                       │ + isClosed │  │  ()      │
                                                       │   (): bool │  └──────────┘
                                                       │ + open()   │
                                                       │ + close()  │
                                                       │ + emergency│
                                                       │   Open()   │
                                                       │ - canOpen()│
                                                       │ - canClose │
                                                       │   ()       │
                                                       └────────────┘

┌──────────────────────────────┐
│      «struct»                │
│    ExternalRequest           │
├──────────────────────────────┤
│ + floor: int                 │
│ + direction: Direction       │
├──────────────────────────────┤
│ + ExternalRequest(int, Dir)  │
│ + operator<(Request): bool   │
└──────────────────────────────┘
```

---

## 🎨 Detailed Class Diagrams by Component

### 1. Core Elevator Class (Detailed)

```
┌────────────────────────────────────────────────────────────────┐
│                         Elevator                               │
├────────────────────────────────────────────────────────────────┤
│ ATTRIBUTES                                                     │
├────────────────────────────────────────────────────────────────┤
│ - id: int                                                      │
│ - currentFloor: int                                            │
│ - minFloor: int                                                │
│ - maxFloor: int                                                │
│ - direction: Direction                                         │
│ - door: unique_ptr<Door>                                       │
│ - display: unique_ptr<Display>                                 │
│ - internalRequests: set<int>                                   │
├────────────────────────────────────────────────────────────────┤
│ PUBLIC METHODS                                                 │
├────────────────────────────────────────────────────────────────┤
│ + Elevator(elevatorId: int, minFloor: int, maxFloor: int)     │
│ + ~Elevator()                                                  │
│                                                                │
│ GETTERS                                                        │
│ + getId(): int {const}                                         │
│ + getCurrentFloor(): int {const}                               │
│ + getDirection(): Direction {const}                            │
│ + isIdle(): bool {const}                                       │
│ + getInternalRequests(): const set<int>& {const}               │
│                                                                │
│ REQUEST MANAGEMENT                                             │
│ + addInternalRequest(floor: int): bool                         │
│ + processNextRequest(): bool                                   │
│                                                                │
│ MOVEMENT                                                       │
│ + moveToFloor(targetFloor: int): void                          │
│                                                                │
│ DOOR OPERATIONS                                                │
│ + openDoor(): void                                             │
│ + closeDoor(): void                                            │
│                                                                │
│ DISPLAY                                                        │
│ + showStatus(): void {const}                                   │
│                                                                │
│ SCHEDULING SUPPORT                                             │
│ + calculateScore(requestFloor: int,                            │
│                 requestDirection: Direction): int {const}      │
├────────────────────────────────────────────────────────────────┤
│ PRIVATE METHODS                                                │
├────────────────────────────────────────────────────────────────┤
│ - determineDirection(): void                                   │
│ - handleDoorOperations(): void                                 │
└────────────────────────────────────────────────────────────────┘
         │                    │
         │ owns               │ owns
         │ 1                  │ 1
         ▼                    ▼
    ┌─────────┐         ┌──────────┐
    │  Door   │         │ Display  │
    └─────────┘         └──────────┘
```

---

### 2. Strategy Pattern - Scheduler Hierarchy

```
                    ┌────────────────────────────────────────┐
                    │       «interface»                      │
                    │     ElevatorScheduler                  │
                    ├────────────────────────────────────────┤
                    │ {abstract}                             │
                    ├────────────────────────────────────────┤
                    │ + selectElevator(                      │
                    │     elevators: const vector<           │
                    │         unique_ptr<Elevator>>&,        │
                    │     request: const ExternalRequest&    │
                    │   ): int {abstract}                    │
                    │                                        │
                    │ + getName(): string {abstract}         │
                    └──────────────┬─────────────────────────┘
                                   │
                                   │ implements
                       ┌───────────┼────────────┐
                       │           │            │
                       ▼           ▼            ▼
        ┌──────────────────┐ ┌──────────────┐ ┌──────────────────┐
        │ FCFSScheduler    │ │SCANScheduler │ │  LOOKScheduler   │
        ├──────────────────┤ ├──────────────┤ ├──────────────────┤
        │                  │ │              │ │                  │
        ├──────────────────┤ ├──────────────┤ ├──────────────────┤
        │+ selectElevator()│ │+ select...() │ │+ selectElevator()│
        │  : int           │ │  : int       │ │  : int           │
        │                  │ │              │ │                  │
        │+ getName()       │ │+ getName()   │ │+ getName()       │
        │  : string        │ │  : string    │ │  : string        │
        │                  │ │              │ │                  │
        │ALGORITHM:        │ │ALGORITHM:    │ │ALGORITHM:        │
        │First Come        │ │Continue in   │ │Like SCAN but     │
        │First Serve       │ │same direction│ │reverses early    │
        └──────────────────┘ └──────────────┘ └──────────────────┘
```

---

### 3. Singleton Pattern - Controller

```
┌───────────────────────────────────────────────────────────────┐
│                    «singleton»                                │
│                 ElevatorController                            │
├───────────────────────────────────────────────────────────────┤
│ STATIC MEMBERS                                                │
├───────────────────────────────────────────────────────────────┤
│ - instance: ElevatorController* {static}                      │
├───────────────────────────────────────────────────────────────┤
│ INSTANCE MEMBERS                                              │
├───────────────────────────────────────────────────────────────┤
│ - elevators: vector<unique_ptr<Elevator>>                     │
│ - externalRequests: set<ExternalRequest>                      │
│ - scheduler: unique_ptr<ElevatorScheduler>                    │
│ - minFloor: int                                               │
│ - maxFloor: int                                               │
├───────────────────────────────────────────────────────────────┤
│ CONSTRUCTOR (PRIVATE)                                         │
├───────────────────────────────────────────────────────────────┤
│ - ElevatorController(numElevators: int,                       │
│                      minFloor: int,                           │
│                      maxFloor: int)                           │
├───────────────────────────────────────────────────────────────┤
│ PUBLIC STATIC METHODS                                         │
├───────────────────────────────────────────────────────────────┤
│ + getInstance(numElevators: int = 3,                          │
│               minFloor: int = 0,                              │
│               maxFloor: int = 10): ElevatorController* {static}│
│                                                               │
│ + destroyInstance(): void {static}                            │
├───────────────────────────────────────────────────────────────┤
│ PUBLIC METHODS                                                │
├───────────────────────────────────────────────────────────────┤
│ + ~ElevatorController()                                       │
│                                                               │
│ STRATEGY PATTERN                                              │
│ + setScheduler(scheduler: unique_ptr<ElevatorScheduler>): void│
│                                                               │
│ REQUEST MANAGEMENT                                            │
│ + addExternalRequest(floor: int, direction: Direction): bool │
│ + processRequests(): void                                     │
│ + processInternalRequests(): void                             │
│                                                               │
│ MONITORING                                                    │
│ + displayStatus(): void {const}                               │
│ + getNumElevators(): int {const}                              │
│ + getElevator(index: int): Elevator*                          │
│ + getPendingRequestsCount(): int {const}                      │
├───────────────────────────────────────────────────────────────┤
│ PRIVATE METHODS                                               │
├───────────────────────────────────────────────────────────────┤
│ - isValidFloor(floor: int): bool {const}                      │
│ - processExternalRequest(request: ExternalRequest): bool      │
├───────────────────────────────────────────────────────────────┤
│ DELETED METHODS (Singleton)                                   │
├───────────────────────────────────────────────────────────────┤
│ - ElevatorController(const ElevatorController&) = delete      │
│ - operator=(const ElevatorController&) = delete               │
└───────────────────────────────────────────────────────────────┘
```

---

## 🔗 Relationship Diagrams

### 1. Composition Relationships

```
ElevatorController
        │
        │ owns (composition)
        │ multiplicity: 1..*
        ▼
    Elevator ────────────────────┐
        │                        │
        │ owns                   │ owns
        │ 1                      │ 1
        ▼                        ▼
      Door                    Display

Legend:
─────> : Strong ownership (composition)
       Object cannot exist without owner
       Owner manages lifecycle
```

---

### 2. Association Relationships

```
ElevatorController
        │
        │ uses
        │ 1
        ▼
  ElevatorScheduler ◄─────── (Strategy Pattern)
        △
        │ implements
        │
   ┌────┼────┐
   │    │    │
FCFS SCAN LOOK


ElevatorController
        │
        │ manages
        │ 1..*
        ▼
  ExternalRequest


Legend:
- - - -> : Dependency (uses)
──────> : Association
◄─────  : Realization (implements)
```

---

### 3. Complete Relationship Diagram

```
┌──────────────────────┐
│ ElevatorController   │
│    «singleton»       │
└──────┬───────────────┘
       │
       │ contains (1..*)
       │ ◆───────────────────────┐
       │                         │
       │ uses (1)                │
       │ ◇─────────────┐         │
       │               │         │
       ▼               ▼         ▼
┌─────────────┐  ┌──────────┐  ┌────────────────┐
│ExternalReq  │  │Scheduler │  │   Elevator     │
│  «struct»   │  │«interface»  │                │
└─────────────┘  └────△─────┘  └───┬────────────┘
                      │            │
              ┌───────┼────────┐   │ owns (1)
              │       │        │   │ ◆──────────┐
              ▼       ▼        ▼   │            │
          ┌────┐  ┌────┐  ┌────┐  │            │
          │FCFS│  │SCAN│  │LOOK│  ▼            ▼
          └────┘  └────┘  └────┘ ┌────┐    ┌───────┐
                                 │Door│    │Display│
                                 └────┘    └───────┘

Legend:
◆─────> : Composition (strong ownership)
◇─────> : Aggregation (weak ownership)
△       : Inheritance/Realization
```

---

## 📊 Sequence Diagrams

### 1. External Request Flow

```
User    Controller    Scheduler    Elevator    Door    Display
 │          │            │           │          │        │
 │─request─>│            │           │          │        │
 │          │            │           │          │        │
 │          │─validate──>│           │          │        │
 │          │            │           │          │        │
 │          │─select────>│           │          │        │
 │          │  elevator  │           │          │        │
 │          │<───id──────┘           │          │        │
 │          │                        │          │        │
 │          │─addInternal───────────>│          │        │
 │          │   Request              │          │        │
 │          │                        │          │        │
 │          │                        │─moveTo──>│        │
 │          │                        │  Floor   │        │
 │          │                        │          │        │
 │          │                        │─────────>│        │
 │          │                        │   open   │        │
 │          │                        │          │        │
 │          │                        │──────────────────>│
 │          │                        │   showStatus      │
 │          │                        │          │        │
 │          │                        │<─────────┘        │
 │          │                        │          │        │
 │          │                        │─────────>│        │
 │          │                        │   close  │        │
 │<─status──┘                        │          │        │
```

---

### 2. Scheduler Selection Process

```
Controller         FCFSScheduler      SCANScheduler      LOOKScheduler
    │                    │                  │                  │
    │─selectElevator────>│                  │                  │
    │   (Strategy)       │                  │                  │
    │                    │                  │                  │
    │                    │─evaluate─────────┐                  │
    │                    │  elevators       │                  │
    │                    │                  │                  │
    │<──bestElevator─────┘                  │                  │
    │                                       │                  │
    │                                       │                  │
    │  (OR with different scheduler)        │                  │
    │                                       │                  │
    │─setScheduler(new SCANScheduler)──────>│                  │
    │                                       │                  │
    │─selectElevator────────────────────────>│                  │
    │                                       │                  │
    │                                       │─evaluate─────────┐
    │                                       │  with SCAN       │
    │                                       │  algorithm       │
    │                                       │                  │
    │<──────bestElevator────────────────────┘                  │
```

---

### 3. Elevator Movement Sequence

```
Elevator          Door           Display        InternalRequests
   │               │                │                  │
   │─processNext──>│                │                  │
   │  Request      │                │                  │
   │               │                │                  │
   │<──────────────┘                │                  │
   │  (finds next floor)            │                  │
   │                                │                  │
   │─moveToFloor──────────────────────────────────────>│
   │                                │                  │
   │  (while currentFloor != target)                   │
   │  │                             │                  │
   │  │─currentFloor++              │                  │
   │  │                             │                  │
   │  │─showStatus─────────────────>│                  │
   │  │                             │                  │
   │  └─(repeat)                    │                  │
   │                                │                  │
   │  (arrived at floor)            │                  │
   │                                │                  │
   │────────────>│                  │                  │
   │    open     │                  │                  │
   │             │                  │                  │
   │<────────────┘                  │                  │
   │                                │                  │
   │  (wait for passengers)         │                  │
   │                                │                  │
   │────────────>│                  │                  │
   │    close    │                  │                  │
   │             │                  │                  │
   │<────────────┘                  │                  │
   │                                │                  │
   │─removeRequest─────────────────────────────────────>│
   │                                │                  │
```

---

## 🎭 State Diagrams

### 1. Elevator State Machine

```
                    ┌─────────────┐
                    │    IDLE     │
                    └──────┬──────┘
                           │
        ┌──────────────────┼──────────────────┐
        │                  │                  │
        │ request          │ request          │
        │ above            │ below            │
        ▼                  ▼                  ▼
   ┌─────────┐      ┌──────────┐      ┌──────────┐
   │ MOVING  │      │   IDLE   │      │ MOVING   │
   │   UP    │      │          │      │  DOWN    │
   └────┬────┘      └──────────┘      └────┬─────┘
        │                                   │
        │ no more requests above            │ no more requests below
        │                                   │
        └───────────────┬───────────────────┘
                        │
                        ▼
                   ┌─────────┐
                   │  IDLE   │
                   └─────────┘

States: UP, DOWN, IDLE (Direction enum)
```

---

### 2. Door State Machine

```
        ┌──────────┐
        │  CLOSED  │
        └────┬─────┘
             │
             │ open()
             ▼
        ┌─────────┐
        │ OPENING │ (transition state)
        └────┬────┘
             │
             │ (timer)
             ▼
        ┌────────┐
        │  OPEN  │
        └────┬───┘
             │
             │ close()
             ▼
        ┌─────────┐
        │ CLOSING │ (transition state)
        └────┬────┘
             │
             │ (timer)
             ▼
        ┌──────────┐
        │  CLOSED  │
        └──────────┘

Emergency: Any state ──emergency()──> OPEN
```

---

## 🏗️ Component Diagram

```
┌────────────────────────────────────────────────────────────┐
│                    ELEVATOR SYSTEM                         │
├────────────────────────────────────────────────────────────┤
│                                                            │
│  ┌───────────────────────────────────────────────────┐    │
│  │         Control Layer                             │    │
│  │  ┌─────────────────────────────────────────┐     │    │
│  │  │      ElevatorController                 │     │    │
│  │  │         (Singleton)                     │     │    │
│  │  └─────────────────────────────────────────┘     │    │
│  └──────────────────┬────────────────────────────────┘    │
│                     │                                     │
│  ┌──────────────────┼────────────────────────────────┐    │
│  │  Strategy Layer  │                                │    │
│  │  ┌───────────────▼──────────────────────────┐     │    │
│  │  │      ElevatorScheduler (Interface)       │     │    │
│  │  ├──────────┬──────────┬───────────────┐    │     │    │
│  │  │   FCFS   │   SCAN   │     LOOK      │    │     │    │
│  │  └──────────┴──────────┴───────────────┘    │     │    │
│  └───────────────────────────────────────────────┘     │    │
│                                                        │    │
│  ┌─────────────────────────────────────────────────┐  │    │
│  │         Business Logic Layer                    │  │    │
│  │  ┌─────────────────────────────┐                │  │    │
│  │  │         Elevator            │                │  │    │
│  │  │  - Movement Logic           │                │  │    │
│  │  │  - Request Management       │                │  │    │
│  │  │  - State Management         │                │  │    │
│  │  └─────────────┬───────────────┘                │  │    │
│  └────────────────┼──────────────────────────────────┘  │    │
│                   │                                     │    │
│  ┌────────────────┼──────────────────────────────────┐  │    │
│  │  Component Layer│                                 │  │    │
│  │  ┌──────────────▼────────┐   ┌────────────────┐  │  │    │
│  │  │         Door          │   │    Display     │  │  │    │
│  │  │  - State Machine      │   │  - Formatting  │  │  │    │
│  │  │  - Door Control       │   │  - Output      │  │  │    │
│  │  └───────────────────────┘   └────────────────┘  │  │    │
│  └───────────────────────────────────────────────────┘  │    │
│                                                          │    │
└──────────────────────────────────────────────────────────┘    │
```

---

## 📦 Package Diagram

```
┌─────────────────────────────────────────────────────────────┐
│                   elevator_system                           │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  ┌────────────────┐        ┌──────────────────┐            │
│  │   controller   │◄───────│   scheduling     │            │
│  │                │ uses   │                  │            │
│  │ - Controller.h │        │ - Scheduler.h    │            │
│  │ - Controller.cpp│       │ - Scheduler.cpp  │            │
│  └────────┬───────┘        └──────────────────┘            │
│           │                                                 │
│           │ manages                                         │
│           ▼                                                 │
│  ┌────────────────┐                                         │
│  │   elevator     │                                         │
│  │                │                                         │
│  │ - Elevator.h   │                                         │
│  │ - Elevator.cpp │                                         │
│  └────────┬───────┘                                         │
│           │                                                 │
│           │ uses                                            │
│           ▼                                                 │
│  ┌────────────────┬──────────────────┐                      │
│  │   components   │                  │                      │
│  │                │                  │                      │
│  │ - Door.h       │  - Display.h     │                      │
│  │ - Door.cpp     │  - Display.cpp   │                      │
│  └────────────────┴──────────────────┘                      │
│                                                             │
│  ┌────────────────────────────────────┐                     │
│  │          utilities                 │                     │
│  │                                    │                     │
│  │  - Direction (enum)                │                     │
│  │  - DoorState (enum)                │                     │
│  │  - ExternalRequest (struct)        │                     │
│  └────────────────────────────────────┘                     │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

---

## 🎯 Design Pattern Visualizations

### 1. Strategy Pattern

```
┌──────────────────────────────────────────────────────────┐
│              STRATEGY PATTERN                            │
├──────────────────────────────────────────────────────────┤
│                                                          │
│  Client: ElevatorController                              │
│     │                                                    │
│     │ uses                                               │
│     ▼                                                    │
│  ┌────────────────────────────┐                          │
│  │  Strategy (Interface)      │                          │
│  │  ElevatorScheduler         │                          │
│  │                            │                          │
│  │ + selectElevator()         │                          │
│  └──────────┬─────────────────┘                          │
│             │                                            │
│             │ implements                                 │
│    ┌────────┼─────────┐                                  │
│    │        │         │                                  │
│    ▼        ▼         ▼                                  │
│  ┌────┐  ┌────┐   ┌────┐                                │
│  │FCFS│  │SCAN│   │LOOK│ Concrete Strategies            │
│  └────┘  └────┘   └────┘                                │
│                                                          │
│  Usage:                                                  │
│  controller->setScheduler(new LOOKScheduler());          │
│  controller->processRequests();                          │
│                                                          │
└──────────────────────────────────────────────────────────┘
```

---

### 2. Singleton Pattern

```
┌──────────────────────────────────────────────────────────┐
│              SINGLETON PATTERN                           │
├──────────────────────────────────────────────────────────┤
│                                                          │
│  ┌────────────────────────────────────┐                  │
│  │    ElevatorController              │                  │
│  ├────────────────────────────────────┤                  │
│  │ - instance: static Controller*     │  ◄───┐           │
│  ├────────────────────────────────────┤      │           │
│  │ - ElevatorController() {private}   │      │ points to │
│  │ + getInstance(): Controller*       │──────┘ self      │
│  │ + destroyInstance()                │                  │
│  └────────────────────────────────────┘                  │
│                                                          │
│  Ensures:                                                │
│  - Only ONE instance exists                              │
│  - Global access point                                   │
│  - Lazy initialization                                   │
│                                                          │
│  Usage:                                                  │
│  ElevatorController* ctrl =                              │
│      ElevatorController::getInstance();                  │
│                                                          │
└──────────────────────────────────────────────────────────┘
```

---

### 3. State Pattern (Door)

```
┌──────────────────────────────────────────────────────────┐
│              STATE PATTERN (Door)                        │
├──────────────────────────────────────────────────────────┤
│                                                          │
│  Context: Door                                           │
│                                                          │
│  States: DoorState enum                                  │
│    - CLOSED                                              │
│    - OPENING                                             │
│    - OPEN                                                │
│    - CLOSING                                             │
│                                                          │
│  Transitions:                                            │
│    CLOSED ─open()─> OPENING ─timer─> OPEN               │
│    OPEN ─close()─> CLOSING ─timer─> CLOSED              │
│                                                          │
│  Behavior changes based on state:                        │
│    - canOpen() : depends on state                        │
│    - canClose() : depends on state                       │
│                                                          │
└──────────────────────────────────────────────────────────┘
```

---

### 4. Composition Pattern

```
┌──────────────────────────────────────────────────────────┐
│           COMPOSITION PATTERN                            │
├──────────────────────────────────────────────────────────┤
│                                                          │
│  Whole: Elevator                                         │
│     │                                                    │
│     │ owns (◆)                                           │
│     │                                                    │
│     ├──────────> Part: Door (1)                          │
│     │            - Cannot exist without Elevator         │
│     │            - Lifecycle managed by Elevator         │
│     │                                                    │
│     └──────────> Part: Display (1)                       │
│                  - Cannot exist without Elevator         │
│                  - Lifecycle managed by Elevator         │
│                                                          │
│  Implementation:                                         │
│  class Elevator {                                        │
│      unique_ptr<Door> door;      // Owns                 │
│      unique_ptr<Display> display; // Owns                │
│  };                                                      │
│                                                          │
└──────────────────────────────────────────────────────────┘
```

---

## 📐 Deployment Diagram

```
┌────────────────────────────────────────────────────────────────┐
│                  DEPLOYMENT VIEW                               │
├────────────────────────────────────────────────────────────────┤
│                                                                │
│  ┌───────────────────────────────────────────────┐             │
│  │  «executable»                                 │             │
│  │  elevator_system                              │             │
│  │                                               │             │
│  │  Contains:                                    │             │
│  │  - main.cpp (entry point)                     │             │
│  │  - ElevatorController (singleton instance)    │             │
│  │  - Elevator instances (3 by default)          │             │
│  │  - Scheduler instance                         │             │
│  │                                               │             │
│  │  Libraries:                                   │             │
│  │  - C++ Standard Library                       │             │
│  │    - <iostream> : I/O                         │             │
│  │    - <vector> : Collections                   │             │
│  │    - <set> : Sorted collections               │             │
│  │    - <memory> : Smart pointers                │             │
│  │    - <thread> : Concurrency                   │             │
│  │    - <chrono> : Timing                        │             │
│  │                                               │             │
│  └───────────────────────────────────────────────┘             │
│                                                                │
└────────────────────────────────────────────────────────────────┘
```

---

## 🎯 Use Case Diagram

```
                        ELEVATOR SYSTEM
                              
    ┌─────────────────────────────────────────────────────┐
    │                                                     │
    │         ┌───────────────────┐                       │
    │         │  Request Elevator │                       │
    │         │  from Floor       │                       │
    │         └─────────┬─────────┘                       │
    │                   │                                 │
    │  ┌────────────────┘                                 │
    │  │                                                  │
┌───┴──▼──┐     ┌──────────────────┐                      │
│  User   │     │ Select           │                      │
│ (Person)│─────│ Destination      │                      │
└───┬──┬──┘     └──────────────────┘                      │
    │  │                                                  │
    │  │        ┌──────────────────┐                      │
    │  └────────│ View Status      │                      │
    │           └──────────────────┘                      │
    │                                                     │
    │                                                     │
┌───┴──────┐    ┌──────────────────┐                      │
│ Building │    │ Change Scheduler │                      │
│ Manager  │────│ Algorithm        │                      │
└───┬──────┘    └──────────────────┘                      │
    │                                                     │
    │           ┌──────────────────┐                      │
    └───────────│ Monitor System   │                      │
                └──────────────────┘                      │
                                                          │
    └─────────────────────────────────────────────────────┘
```

---

## 📊 Object Diagram (Runtime Example)

```
                     Runtime State Example
                              
┌──────────────────────────────────────────────────────────────┐
│                                                              │
│  controller:ElevatorController                               │
│  ─────────────────────────────                               │
│  minFloor = 0                                                │
│  maxFloor = 10                                               │
│  externalRequests = {(5,UP), (7,DOWN)}                       │
│                                                              │
│         │                          │                         │
│         │ elevators[0]             │ scheduler               │
│         ▼                          ▼                         │
│  ┌──────────────────┐       ┌──────────────┐                │
│  │ elevator1        │       │ :LOOK        │                │
│  │ ─────────        │       │ Scheduler    │                │
│  │ id = 1           │       └──────────────┘                │
│  │ currentFloor = 3 │                                        │
│  │ direction = UP   │                                        │
│  │ requests = {8,10}│                                        │
│  └─────┬──────┬─────┘                                        │
│        │      │                                              │
│        │      └──────┐                                       │
│        ▼             ▼                                       │
│  ┌─────────┐   ┌─────────┐                                  │
│  │ :Door   │   │:Display │                                  │
│  │ ─────   │   │ ─────── │                                  │
│  │state=   │   │ id = 1  │                                  │
│  │ CLOSED  │   └─────────┘                                  │
│  └─────────┘                                                │
│                                                              │
│         │ elevators[1]                                       │
│         ▼                                                    │
│  ┌──────────────────┐                                        │
│  │ elevator2        │                                        │
│  │ ─────────        │                                        │
│  │ id = 2           │                                        │
│  │ currentFloor = 7 │                                        │
│  │ direction = IDLE │                                        │
│  │ requests = {}    │                                        │
│  └──────────────────┘                                        │
│                                                              │
└──────────────────────────────────────────────────────────────┘
```

---

## 🎨 Design Patterns Summary UML

```
┌────────────────────────────────────────────────────────────┐
│         DESIGN PATTERNS IN ELEVATOR SYSTEM                 │
├────────────────────────────────────────────────────────────┤
│                                                            │
│  1. STRATEGY PATTERN                                       │
│     ┌──────────────────┐                                   │
│     │ Scheduler        │ ◄─── Interchangeable algorithms  │
│     │ (Interface)      │                                   │
│     └────────┬─────────┘                                   │
│              │ implements                                  │
│        ┌─────┼─────┐                                       │
│        ▼     ▼     ▼                                       │
│      FCFS  SCAN  LOOK                                      │
│                                                            │
│  2. SINGLETON PATTERN                                      │
│     ┌──────────────────┐                                   │
│     │ Controller       │ ◄─── Single instance             │
│     │ - instance       │                                   │
│     │ + getInstance()  │                                   │
│     └──────────────────┘                                   │
│                                                            │
│  3. STATE PATTERN                                          │
│     ┌──────────────────┐                                   │
│     │ Door             │ ◄─── Behavior changes with state │
│     │ - state:         │                                   │
│     │   DoorState      │                                   │
│     └──────────────────┘                                   │
│                                                            │
│  4. COMPOSITION PATTERN                                    │
│     ┌──────────────────┐                                   │
│     │ Elevator         │ ◆──> Door                         │
│     │                  │ ◆──> Display                      │
│     └──────────────────┘                                   │
│                                                            │
└────────────────────────────────────────────────────────────┘
```

---

## 📋 Class Responsibility Collaboration (CRC) Cards

```
┌─────────────────────────────────────────────────────┐
│  Class: ElevatorController                          │
├─────────────────────────────────────────────────────┤
│  Responsibilities:                                  │
│  - Manage all elevators in system                   │
│  - Accept external requests                         │
│  - Coordinate elevator assignment                   │
│  - Delegate to scheduler                            │
│  - Monitor system status                            │
├─────────────────────────────────────────────────────┤
│  Collaborators:                                     │
│  - Elevator (manages multiple)                      │
│  - ElevatorScheduler (uses for decisions)           │
│  - ExternalRequest (queues)                         │
└─────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────┐
│  Class: Elevator                                    │
├─────────────────────────────────────────────────────┤
│  Responsibilities:                                  │
│  - Track current floor and direction                │
│  - Manage internal requests                         │
│  - Execute movement logic                           │
│  - Control door operations                          │
│  - Display status                                   │
├─────────────────────────────────────────────────────┤
│  Collaborators:                                     │
│  - Door (controls)                                  │
│  - Display (shows information)                      │
└─────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────┐
│  Class: ElevatorScheduler (Interface)               │
├─────────────────────────────────────────────────────┤
│  Responsibilities:                                  │
│  - Define scheduling contract                       │
│  - Select best elevator for request                 │
├─────────────────────────────────────────────────────┤
│  Collaborators:                                     │
│  - Elevator (evaluates)                             │
│  - ExternalRequest (considers)                      │
└─────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────┐
│  Class: Door                                        │
├─────────────────────────────────────────────────────┤
│  Responsibilities:                                  │
│  - Manage door state                                │
│  - Execute open/close operations                    │
│  - Validate state transitions                       │
├─────────────────────────────────────────────────────┤
│  Collaborators:                                     │
│  - None (self-contained)                            │
└─────────────────────────────────────────────────────┘
```

---

## 🎯 Summary

This UML documentation provides:

✅ **Complete Class Diagram** with all classes and relationships
✅ **Sequence Diagrams** showing request flow and interactions
✅ **State Diagrams** for Elevator and Door states
✅ **Component Diagram** showing system layers
✅ **Package Diagram** showing code organization
✅ **Design Pattern Visualizations** for all 4 patterns used
✅ **CRC Cards** defining responsibilities
✅ **Object Diagram** showing runtime example

### Key Relationships:
- **Composition** (◆): Elevator owns Door and Display
- **Association**: Controller manages Elevators
- **Realization** (△): Concrete schedulers implement interface
- **Dependency**: Controller uses Scheduler

### Multiplicity:
- Controller → Elevator: `1..*` (one-to-many)
- Elevator → Door: `1` (one-to-one)
- Elevator → Display: `1` (one-to-one)
- Controller → Scheduler: `1` (one-to-one)

Perfect for interviews and documentation! 🚀
