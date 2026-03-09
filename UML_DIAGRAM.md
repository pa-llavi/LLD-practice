# Snake and Ladder - UML Diagrams

## Table of Contents
1. [Class Diagram (Mermaid)](#class-diagram-mermaid)
2. [Class Diagram (PlantUML)](#class-diagram-plantuml)
3. [Class Diagram (ASCII)](#class-diagram-ascii)
4. [Sequence Diagram](#sequence-diagram)
5. [Relationship Explanation](#relationship-explanation)

---

## Class Diagram (Mermaid)

```mermaid
classDiagram
    class Snake {
        -int head
        -int tail
        +Snake(int head, int tail)
        +int getHead() const
        +int getTail() const
        +bool hasSnakeAt(int position) const
    }

    class Ladder {
        -int start
        -int end
        +Ladder(int start, int end)
        +int getStart() const
        +int getEnd() const
        +bool hasLadderAt(int position) const
    }

    class Dice {
        -int numberOfDice
        -int facesPerDie
        -mt19937 generator
        +Dice()
        +Dice(int numberOfDice, int facesPerDie)
        +int roll()
    }

    class Player {
        -string name
        -int position
        +Player(string name)
        +string getName() const
        +int getPosition() const
        +void setPosition(int position)
        +void move(int steps)
    }

    class Board {
        -int size
        -vector~Snake~ snakes
        -vector~Ladder~ ladders
        -unordered_map~int,int~ snakeMap
        -unordered_map~int,int~ ladderMap
        +Board()
        +Board(int size)
        +void addSnake(Snake snake)
        +void addLadder(Ladder ladder)
        +int getNewPositionAfterSnakeOrLadder(int pos) const
        +int getSize() const
        +vector~Snake~ getSnakes() const
        +vector~Ladder~ getLadders() const
    }

    class Game {
        -Board board
        -Dice dice
        -vector~Player*~ players
        -queue~Player*~ playerQueue
        -GameStatus status
        -Player* winner
        -bool exactWinRequired
        +Game()
        +Game(Board board)
        +void addPlayer(string name)
        +void setExactWinRequired(bool required)
        +void startGame()
        +string playTurn()
        +void playGame()
        +GameStatus getStatus() const
        +Player* getWinner() const
    }

    class GameStatus {
        <<enumeration>>
        NOT_STARTED
        IN_PROGRESS
        FINISHED
    }

    Game *-- Board : contains
    Game *-- Dice : contains
    Game o-- Player : manages
    Game -- GameStatus : uses
    Board o-- Snake : contains
    Board o-- Ladder : contains
```

---

## Class Diagram (PlantUML)

```plantuml
@startuml Snake_And_Ladder_Class_Diagram

' Class definitions

class Snake {
    - head : int
    - tail : int
    + Snake(head : int, tail : int)
    + getHead() : int
    + getTail() : int
    + hasSnakeAt(position : int) : bool
}

class Ladder {
    - start : int
    - end : int
    + Ladder(start : int, end : int)
    + getStart() : int
    + getEnd() : int
    + hasLadderAt(position : int) : bool
}

class Dice {
    - numberOfDice : int
    - facesPerDie : int
    - generator : mt19937
    + Dice()
    + Dice(numberOfDice : int, facesPerDie : int)
    + roll() : int
}

class Player {
    - name : string
    - position : int
    + Player(name : string)
    + getName() : string
    + getPosition() : int
    + setPosition(position : int) : void
    + move(steps : int) : void
}

class Board {
    - size : int
    - snakes : vector<Snake>
    - ladders : vector<Ladder>
    - snakeMap : unordered_map<int,int>
    - ladderMap : unordered_map<int,int>
    + Board()
    + Board(size : int)
    + addSnake(snake : Snake) : void
    + addLadder(ladder : Ladder) : void
    + getNewPositionAfterSnakeOrLadder(pos : int) : int
    + getSize() : int
    + getSnakes() : vector<Snake>
    + getLadders() : vector<Ladder>
}

class Game {
    - board : Board
    - dice : Dice
    - players : vector<shared_ptr<Player>>
    - playerQueue : queue<shared_ptr<Player>>
    - status : GameStatus
    - winner : shared_ptr<Player>
    - exactWinRequired : bool
    + Game()
    + Game(board : Board)
    + addPlayer(name : string) : void
    + setExactWinRequired(required : bool) : void
    + startGame() : void
    + playTurn() : string
    + playGame() : void
    + getStatus() : GameStatus
    + getWinner() : shared_ptr<Player>
}

enum GameStatus {
    NOT_STARTED
    IN_PROGRESS
    FINISHED
}

' Relationships

Game *-- Board : contains
Game *-- Dice : contains
Game o-- Player : manages
Game -- GameStatus : uses

Board o-- Snake : contains 0..*
Board o-- Ladder : contains 0..*

note right of Game
  Facade Pattern:
  Orchestrates all game logic
  and manages player turns
end note

note right of Board
  Dual Storage:
  - Vector for iteration
  - Map for O(1) lookup
end note

note left of Snake
  Value Object:
  Immutable after creation
end note

note left of Ladder
  Value Object:
  Immutable after creation
end note

@enduml
```

---

## Class Diagram (ASCII)

```
┌──────────────────────────────────────────────────────────────────┐
│                             GAME                                  │
│ ──────────────────────────────────────────────────────────────── │
│ - board : Board                                                   │
│ - dice : Dice                                                     │
│ - players : vector<shared_ptr<Player>>                            │
│ - playerQueue : queue<shared_ptr<Player>>                         │
│ - status : GameStatus                                             │
│ - winner : shared_ptr<Player>                                     │
│ - exactWinRequired : bool                                         │
│ ──────────────────────────────────────────────────────────────── │
│ + Game()                                                          │
│ + Game(board : Board)                                             │
│ + addPlayer(name : string) : void                                 │
│ + setExactWinRequired(required : bool) : void                     │
│ + startGame() : void                                              │
│ + playTurn() : string                                             │
│ + playGame() : void                                               │
│ + getStatus() : GameStatus                                        │
│ + getWinner() : shared_ptr<Player>                                │
└──────────┬────────────┬─────────────┬──────────────────────────────┘
           │            │             │
           │(contains)  │(contains)   │(manages)
           │            │             │
      ┌────▼──────┐ ┌──▼───────┐ ┌───▼──────────────┐
      │   BOARD   │ │   DICE   │ │     PLAYER       │
      └───────────┘ └──────────┘ └──────────────────┘
           │
           │(contains)
           │
    ┌──────┴─────────┐
    │                │
┌───▼──────┐    ┌────▼──────┐
│  SNAKE   │    │  LADDER   │
└──────────┘    └───────────┘


╔═══════════════════════════════════╗
║           SNAKE                   ║
╠═══════════════════════════════════╣
║ - head : int                      ║
║ - tail : int                      ║
╠═══════════════════════════════════╣
║ + Snake(head, tail)               ║
║ + getHead() : int                 ║
║ + getTail() : int                 ║
║ + hasSnakeAt(pos) : bool          ║
╚═══════════════════════════════════╝


╔═══════════════════════════════════╗
║           LADDER                  ║
╠═══════════════════════════════════╣
║ - start : int                     ║
║ - end : int                       ║
╠═══════════════════════════════════╣
║ + Ladder(start, end)              ║
║ + getStart() : int                ║
║ + getEnd() : int                  ║
║ + hasLadderAt(pos) : bool         ║
╚═══════════════════════════════════╝


╔═══════════════════════════════════╗
║           DICE                    ║
╠═══════════════════════════════════╣
║ - numberOfDice : int              ║
║ - facesPerDie : int               ║
║ - generator : mt19937             ║
╠═══════════════════════════════════╣
║ + Dice()                          ║
║ + Dice(num, faces)                ║
║ + roll() : int                    ║
╚═══════════════════════════════════╝


╔═══════════════════════════════════╗
║           PLAYER                  ║
╠═══════════════════════════════════╣
║ - name : string                   ║
║ - position : int                  ║
╠═══════════════════════════════════╣
║ + Player(name)                    ║
║ + getName() : string              ║
║ + getPosition() : int             ║
║ + setPosition(pos) : void         ║
║ + move(steps) : void              ║
╚═══════════════════════════════════╝


╔═══════════════════════════════════════════════════════╗
║                    BOARD                              ║
╠═══════════════════════════════════════════════════════╣
║ - size : int                                          ║
║ - snakes : vector<Snake>                              ║
║ - ladders : vector<Ladder>                            ║
║ - snakeMap : unordered_map<int,int>                   ║
║ - ladderMap : unordered_map<int,int>                  ║
╠═══════════════════════════════════════════════════════╣
║ + Board()                                             ║
║ + Board(size)                                         ║
║ + addSnake(snake) : void                              ║
║ + addLadder(ladder) : void                            ║
║ + getNewPositionAfterSnakeOrLadder(pos) : int         ║
║ + getSize() : int                                     ║
║ + getSnakes() : vector<Snake>                         ║
║ + getLadders() : vector<Ladder>                       ║
╚═══════════════════════════════════════════════════════╝


╔═══════════════════════╗
║   GameStatus (enum)   ║
╠═══════════════════════╣
║ NOT_STARTED           ║
║ IN_PROGRESS           ║
║ FINISHED              ║
╚═══════════════════════╝
```

---

## Sequence Diagram

### Game Initialization and Single Turn

```mermaid
sequenceDiagram
    participant Client
    participant Game
    participant Board
    participant Dice
    participant Player
    
    Client->>Game: Game()
    Game->>Board: Board(100)
    Game->>Dice: Dice()
    
    Client->>Board: addSnake(Snake(99,54))
    Board->>Board: validate
    Board-->>Client: OK
    
    Client->>Board: addLadder(Ladder(6,25))
    Board->>Board: validate
    Board-->>Client: OK
    
    Client->>Game: addPlayer("Alice")
    Game->>Player: new Player("Alice")
    Player-->>Game: player object
    
    Client->>Game: addPlayer("Bob")
    Game->>Player: new Player("Bob")
    Player-->>Game: player object
    
    Client->>Game: startGame()
    Game->>Game: initializePlayerQueue()
    Game->>Game: status = IN_PROGRESS
    
    Client->>Game: playTurn()
    Game->>Game: get current player
    Game->>Dice: roll()
    Dice-->>Game: 4
    Game->>Player: getPosition()
    Player-->>Game: 0
    Game->>Player: setPosition(4)
    Game->>Board: getNewPositionAfterSnakeOrLadder(4)
    Board-->>Game: 4 (no snake/ladder)
    Game->>Game: check win condition
    Game->>Game: add player to queue
    Game-->>Client: "Alice rolled 4..."
```

### Complete Game Flow

```plantuml
@startuml Snake_And_Ladder_Sequence

actor User
participant "game:Game" as Game
participant "board:Board" as Board
participant "dice:Dice" as Dice
participant "player:Player" as Player

User -> Game : new Game()
activate Game
Game -> Board : new Board(100)
activate Board
Board --> Game : board
deactivate Board
Game -> Dice : new Dice()
activate Dice
Dice --> Game : dice
deactivate Dice
Game --> User : game
deactivate Game

User -> Board : addSnake(Snake(99,54))
activate Board
Board -> Board : validate()
Board --> User : OK
deactivate Board

User -> Board : addLadder(Ladder(6,25))
activate Board
Board -> Board : validate()
Board --> User : OK
deactivate Board

User -> Game : addPlayer("Alice")
activate Game
Game -> Player : new Player("Alice")
activate Player
Player --> Game : player
deactivate Player
Game --> User : OK
deactivate Game

User -> Game : startGame()
activate Game
Game -> Game : initializePlayerQueue()
Game -> Game : status = IN_PROGRESS
Game --> User : OK
deactivate Game

loop Until someone wins
    User -> Game : playTurn()
    activate Game
    
    Game -> Game : currentPlayer = queue.front()
    
    Game -> Dice : roll()
    activate Dice
    Dice --> Game : diceValue
    deactivate Dice
    
    Game -> Player : getPosition()
    activate Player
    Player --> Game : currentPos
    deactivate Player
    
    Game -> Player : setPosition(newPos)
    activate Player
    Player --> Game : OK
    deactivate Player
    
    Game -> Board : getNewPositionAfterSnakeOrLadder(newPos)
    activate Board
    Board --> Game : finalPos
    deactivate Board
    
    alt Hit Snake
        Game -> Player : setPosition(snakeTail)
    else Hit Ladder
        Game -> Player : setPosition(ladderEnd)
    end
    
    Game -> Game : checkWin()
    
    alt Player Won
        Game -> Game : status = FINISHED
        Game -> Game : winner = currentPlayer
    else Game Continues
        Game -> Game : queue.push(currentPlayer)
    end
    
    Game --> User : turnDescription
    deactivate Game
end

@enduml
```

---

## Relationship Explanation

### 1. **Game ◆── Board** (Composition)
- **Type**: Strong ownership (composition)
- **Multiplicity**: 1 to 1
- **Lifetime**: Board cannot exist without Game
- **Symbol**: ◆ (filled diamond)

```cpp
class Game {
    Board board;  // Game owns the board
};
```

### 2. **Game ◆── Dice** (Composition)
- **Type**: Strong ownership
- **Multiplicity**: 1 to 1
- **Lifetime**: Dice is created with Game
- **Symbol**: ◆ (filled diamond)

```cpp
class Game {
    Dice dice;  // Game owns the dice
};
```

### 3. **Game ◇── Player** (Aggregation)
- **Type**: Weak ownership (aggregation)
- **Multiplicity**: 1 to many (2..*)
- **Lifetime**: Players can exist independently
- **Symbol**: ◇ (hollow diamond)

```cpp
class Game {
    vector<shared_ptr<Player>> players;  // Game manages players
};
```

### 4. **Board ◇── Snake** (Aggregation)
- **Type**: Weak ownership
- **Multiplicity**: 1 to many (0..*)
- **Lifetime**: Snakes conceptually independent
- **Symbol**: ◇ (hollow diamond)

```cpp
class Board {
    vector<Snake> snakes;
    unordered_map<int, int> snakeMap;
};
```

### 5. **Board ◇── Ladder** (Aggregation)
- **Type**: Weak ownership
- **Multiplicity**: 1 to many (0..*)
- **Lifetime**: Ladders conceptually independent
- **Symbol**: ◇ (hollow diamond)

```cpp
class Board {
    vector<Ladder> ladders;
    unordered_map<int, int> ladderMap;
};
```

### 6. **Game ── GameStatus** (Dependency)
- **Type**: Usage dependency
- **Multiplicity**: 1 to 1
- **Symbol**: ── (simple line)

```cpp
class Game {
    GameStatus status;  // Game uses enum
};
```

---

## Relationship Summary Table

| From | To | Relationship | Type | Multiplicity | Symbol |
|------|-----|-------------|------|--------------|--------|
| Game | Board | Contains | Composition | 1:1 | ◆── |
| Game | Dice | Contains | Composition | 1:1 | ◆── |
| Game | Player | Manages | Aggregation | 1:* | ◇── |
| Game | GameStatus | Uses | Dependency | 1:1 | ── |
| Board | Snake | Contains | Aggregation | 1:* | ◇── |
| Board | Ladder | Contains | Aggregation | 1:* | ◇── |

---

## Design Patterns Visible in UML

### 1. **Facade Pattern**
```
Game acts as facade for:
- Board
- Dice  
- Players
- Game rules
```

**UML**: Game class with composition relationships to Board, Dice

### 2. **Value Object Pattern**
```
Snake and Ladder are immutable value objects:
- No setters
- Validation in constructor
- Only getters
```

**UML**: Snake and Ladder with private fields, public getters only

### 3. **State Pattern (Implicit)**
```
GameStatus enum manages game states:
- NOT_STARTED
- IN_PROGRESS
- FINISHED
```

**UML**: GameStatus enumeration associated with Game

---

## Key UML Annotations

### Visibility Markers
- `+` = public
- `-` = private
- `#` = protected

### Multiplicity
- `1` = exactly one
- `*` = zero or more
- `0..*` = zero or more (explicit)
- `1..*` = one or more
- `2..*` = two or more

### Relationship Types
- `──────` = Association
- `◇─────` = Aggregation (hollow diamond)
- `◆─────` = Composition (filled diamond)
- `◄─────` = Dependency (dashed arrow)
- `◄═════` = Inheritance (hollow triangle)

---

## How to Use These Diagrams

### For Mermaid:
1. Copy to markdown file
2. View in GitHub, GitLab, or VS Code with Mermaid extension

### For PlantUML:
1. Save to `.puml` file
2. Use PlantUML plugin in VS Code, IntelliJ, or online editor
3. Or visit: http://www.plantuml.com/plantuml/uml/

### For ASCII:
1. Copy to any text file or documentation
2. View in monospace font

---

## Complete System Overview

```
┌─────────────────────────────────────────────────────────┐
│                    CLIENT CODE                          │
│                      (main.cpp)                         │
└────────────────────┬────────────────────────────────────┘
                     │
                     │ creates & uses
                     ▼
         ┏━━━━━━━━━━━━━━━━━━━━━━━━┓
         ┃        GAME            ┃  ◄── Facade Pattern
         ┃  (Orchestrator)        ┃
         ┗━━━━━━━━━┳━━━━━━━━━━━━━━┛
                   │
         ┌─────────┼─────────┬──────────┐
         │         │         │          │
         ▼         ▼         ▼          ▼
    ┌────────┐ ┌──────┐ ┌────────┐ ┌─────────┐
    │ BOARD  │ │ DICE │ │ PLAYER │ │ STATUS  │
    └───┬────┘ └──────┘ └────────┘ └─────────┘
        │
        ├──── SNAKE (0..*)
        └──── LADDER (0..*)
```

---

## Notes for Implementation

1. **Composition vs Aggregation**:
   - Use composition (◆) when child cannot exist without parent
   - Use aggregation (◇) when child can exist independently

2. **Smart Pointers in C++**:
   - `shared_ptr<Player>` represents shared ownership
   - Affects whether relationship is composition or aggregation

3. **const Methods**:
   - Getters marked with `const` in class diagram
   - Indicates method doesn't modify object state

4. **Private Methods**:
   - `initializePlayerQueue()`, `validate()` etc. are private
   - Implementation details, not part of public interface

---

## Summary

This UML design shows:
- ✅ Clear separation of concerns
- ✅ Game as central orchestrator (Facade)
- ✅ Board managing game elements
- ✅ Proper use of composition and aggregation
- ✅ Value objects (Snake, Ladder) with minimal interface
- ✅ State management via enum

The design is **clean**, **modular**, and follows **SOLID principles**.
