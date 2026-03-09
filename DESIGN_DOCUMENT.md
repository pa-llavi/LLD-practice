# Snake and Ladder Game - Low Level Design Document

## Table of Contents
1. [Overview](#overview)
2. [Design Principles](#design-principles)
3. [Class Diagram](#class-diagram)
4. [Detailed Component Design](#detailed-component-design)
5. [Design Patterns Used](#design-patterns-used)
6. [Data Structures and Algorithms](#data-structures-and-algorithms)
7. [Trade-offs and Design Decisions](#trade-offs-and-design-decisions)
8. [Extensibility](#extensibility)

---

## Overview

This is a Low Level Design (LLD) implementation of the classic Snake and Ladder board game in C++. The design follows SOLID principles and uses modern C++ features for a clean, maintainable, and extensible codebase.

### Key Features
- Object-oriented design with clear separation of concerns
- Configurable board size, snakes, and ladders
- Support for multiple players
- Configurable game rules (exact win requirement)
- Both automated and manual game modes
- Comprehensive input validation and error handling

---

## Design Principles

### 1. **Single Responsibility Principle (SRP)**
Each class has a single, well-defined responsibility:
- `Snake`: Represents a snake entity
- `Ladder`: Represents a ladder entity
- `Dice`: Handles dice rolling logic
- `Player`: Manages player state
- `Board`: Manages board configuration and snake/ladder placement
- `Game`: Orchestrates game flow and rules

### 2. **Open/Closed Principle (OCP)**
The design is open for extension but closed for modification:
- New game rules can be added without modifying existing classes
- Board can support different sizes without code changes
- Dice can be configured for different numbers and faces

### 3. **Liskov Substitution Principle (LSP)**
- No inheritance hierarchy that would violate LSP
- All classes can be used polymorphically if needed in future

### 4. **Interface Segregation Principle (ISP)**
- Classes expose only the methods clients need
- No fat interfaces forcing implementation of unused methods

### 5. **Dependency Inversion Principle (DIP)**
- High-level modules (Game) depend on abstractions
- Could be extended with interfaces for testing

---

## Class Diagram

```
┌──────────────┐
│    Game      │
├──────────────┤
│- board       │◆────────┐
│- dice        │◆───┐    │
│- players     │◆─┐ │    │
│- status      │  │ │    │
│- winner      │  │ │    │
└──────────────┘  │ │    │
                  │ │    │
        ┌─────────┘ │    │
        │           │    │
   ┌────▼────┐ ┌───▼──┐ │
   │ Player  │ │ Dice │ │
   ├─────────┤ ├──────┤ │
   │- name   │ │- gen │ │
   │- pos    │ └──────┘ │
   └─────────┘          │
                        │
                   ┌────▼──────┐
                   │   Board   │
                   ├───────────┤
                   │- size     │
                   │- snakes   │◆─┐
                   │- ladders  │◆─┤
                   │- snakeMap │  │
                   │- ladderMap│  │
                   └───────────┘  │
                                  │
                   ┌──────────────┴──────────┐
                   │                         │
              ┌────▼────┐              ┌────▼────┐
              │  Snake  │              │ Ladder  │
              ├─────────┤              ├─────────┤
              │- head   │              │- start  │
              │- tail   │              │- end    │
              └─────────┘              └─────────┘
```

---

## Detailed Component Design

### 1. Snake Class

**Purpose**: Represents a snake on the board

**Design Decisions**:
- **Immutable after creation**: Snake positions cannot change during gameplay
- **Validation in constructor**: Ensures head > tail and positions are positive
- **Simple data holder**: Follows data class pattern with getters

**Why separate class?**
- Encapsulates snake-specific validation logic
- Makes code more readable (Snake vs just two integers)
- Allows future extensions (e.g., snake types, special effects)

```cpp
class Snake {
    int head;  // Higher position
    int tail;  // Lower position
    // Validation ensures head > tail
};
```

### 2. Ladder Class

**Purpose**: Represents a ladder on the board

**Design Decisions**:
- Mirror design of Snake for consistency
- Validation ensures start < end
- Separate from Snake despite similarity (different semantics)

**Why not combine Snake and Ladder?**
- Different business logic (one goes up, one goes down)
- More readable code
- Easier to extend with specific features later

### 3. Dice Class

**Purpose**: Handles random number generation for dice rolls

**Design Decisions**:
- **Modern C++ random**: Uses `std::mt19937` (Mersenne Twister) instead of `rand()`
  - Better randomness quality
  - Thread-safe with proper seeding
  
- **Configurable**: Supports multiple dice and custom faces
  - Default: 1 die with 6 faces
  - Can create 2D6, 3D6, etc.

- **Returns sum**: Simplifies game logic
  - Could be extended to return individual rolls if needed

**Why encapsulate dice logic?**
- Single place for random number generation
- Easy to test (can mock for testing)
- Can change RNG implementation without affecting game logic

```cpp
class Dice {
    std::mt19937 generator;  // High-quality RNG
    int roll() {
        // Returns sum of all dice
    }
};
```

### 4. Player Class

**Purpose**: Represents a player in the game

**Design Decisions**:
- **Simple state management**: Name (immutable) + Position (mutable)
- **Position starts at 0**: Represents "not on board yet"
- **Validation**: Name cannot be empty, position cannot be negative

**Why minimal logic?**
- Player is primarily a data holder
- Game class handles game rules
- Keeps Player class focused and testable

### 5. Board Class

**Purpose**: Manages the game board, snakes, and ladders

**Design Decisions**:

#### Data Structure Choice: HashMap for O(1) Lookup
```cpp
std::unordered_map<int, int> snakeMap;   // head -> tail
std::unordered_map<int, int> ladderMap;  // start -> end
```

**Why HashMap?**
- O(1) average-case lookup when player lands on a position
- Trade memory for speed (acceptable for board size ≤ 100)
- Alternative: Linear search through vectors would be O(n)

#### Validation Strategy
- **At insertion time**: Validates snakes/ladders don't overlap
- **Prevents invalid states**: Can't have snake and ladder at same position
- **Immutable after setup**: Snakes and ladders can't change during game

**Why separate vectors and maps?**
- Vectors: For iteration (displaying board setup)
- Maps: For fast lookup during gameplay
- Dual storage is worth the small memory cost

### 6. Game Class

**Purpose**: Orchestrates the entire game (Facade pattern)

**Design Decisions**:

#### Turn Management: Queue-based
```cpp
std::queue<std::shared_ptr<Player>> playerQueue;
```

**Why queue?**
- Natural FIFO structure for turn rotation
- Simple: pop front, do turn, push back
- Alternative: Round-robin with index (more complex)

#### Player Storage: Shared Pointers
```cpp
std::vector<std::shared_ptr<Player>> players;
```

**Why shared_ptr?**
- Multiple data structures reference same players (vector + queue)
- Automatic memory management
- Prevents dangling pointers

#### Game State Machine
```cpp
enum class GameStatus {
    NOT_STARTED,    // Initial state
    IN_PROGRESS,    // Game running
    FINISHED        // Game over
};
```

**Why state enum?**
- Prevents invalid operations (e.g., adding players after start)
- Clear state transitions
- Easy to validate preconditions

#### Configurable Rules
```cpp
bool exactWinRequired;  // Need exact roll to reach finish?
```

**Why configurable?**
- Different game variants have different rules
- Easy to extend with more rule options
- No code change needed for rule variations

---

## Design Patterns Used

### 1. **Facade Pattern**
- **Where**: `Game` class
- **Why**: Provides simple interface to complex subsystem
- **Benefit**: Client only needs to interact with Game, not all components

### 2. **Builder Pattern (Implicit)**
- **Where**: Board construction with addSnake/addLadder
- **Why**: Allows incremental board configuration
- **Benefit**: Flexible board setup, validation at each step

### 3. **Strategy Pattern (Potential)**
- **Where**: Win condition checking
- **Why**: Different win rules can be strategies
- **Benefit**: Easy to add new win conditions

### 4. **Value Object Pattern**
- **Where**: Snake, Ladder, Player
- **Why**: Immutable data holders with validation
- **Benefit**: Thread-safe, predictable behavior

---

## Data Structures and Algorithms

### 1. Board Position Lookup
**Problem**: Quickly find if position has snake/ladder

**Solution**: HashMap
```cpp
std::unordered_map<int, int> snakeMap;
```

**Complexity**:
- Lookup: O(1) average case
- Insertion: O(1) average case
- Space: O(n + m) where n = snakes, m = ladders

**Alternatives Considered**:
| Approach | Lookup Time | Space | Notes |
|----------|-------------|-------|-------|
| Vector linear search | O(n) | O(n) | Too slow for repeated lookups |
| Sorted vector + binary search | O(log n) | O(n) | Better but still slower |
| HashMap | O(1) avg | O(n) | Best for this use case |
| Array[100] | O(1) | O(k) | Wastes space, k = board size |

**Decision**: HashMap wins for balance of speed and reasonable memory usage

### 2. Turn Management
**Problem**: Rotate turns among players

**Solution**: Queue (FIFO)
```cpp
std::queue<std::shared_ptr<Player>> playerQueue;
```

**Algorithm**:
1. Pop front (current player)
2. Player takes turn
3. If not winner, push to back
4. Repeat

**Complexity**:
- Each operation: O(1)
- Fair rotation guaranteed

**Alternatives Considered**:
- Circular array with index: More complex, same performance
- Vector with iterator: Need manual wraparound logic

### 3. Random Number Generation
**Problem**: Generate random dice rolls

**Solution**: Mersenne Twister (mt19937)
```cpp
std::mt19937 generator;
std::uniform_int_distribution<int> distribution(1, 6);
```

**Why mt19937?**
- Period: 2^19937 - 1 (extremely long)
- Passes statistical tests for randomness
- Standard in modern C++

**Alternatives Considered**:
- `rand()`: Poor quality, not thread-safe
- Linear congruential generator: Too predictable
- Cryptographic RNG: Overkill for a game

---

## Trade-offs and Design Decisions

### 1. **Memory vs Speed: Dual Storage in Board**

**Decision**: Store both vectors and hashmaps

**Trade-off**:
- **Memory Cost**: ~2x storage for snakes/ladders
- **Speed Benefit**: O(1) lookup instead of O(n)
- **Code Clarity**: Separate concerns (iteration vs lookup)

**Why this choice?**
- For typical board (10-20 snakes/ladders), memory cost is negligible
- Lookup happens every turn (high frequency)
- Iteration only for display (low frequency)
- **Verdict**: Speed wins, memory cost acceptable

### 2. **Exact Win Rule: Configurable vs Hardcoded**

**Decision**: Make it configurable

**Trade-off**:
- **Complexity**: Adds a boolean flag and conditional logic
- **Flexibility**: Supports different game variants
- **Testing**: Need to test both modes

**Why this choice?**
- Real-world games have different rules
- Minimal code complexity
- Demonstrates design for extensibility
- **Verdict**: Flexibility worth small complexity

### 3. **Player Management: Queue vs Index**

**Decision**: Use queue for turn management

**Trade-off**:
| Approach | Pros | Cons |
|----------|------|------|
| Queue | Natural FIFO, simple logic | Duplicate references |
| Index | Single storage | Manual wraparound, less clear |

**Why queue?**
- Code clarity: Queue is self-documenting
- Correctness: No off-by-one errors
- Performance: Both O(1), no difference
- **Verdict**: Clarity wins

### 4. **Input Validation: Constructor vs Setter**

**Decision**: Validate in constructor

**Trade-off**:
- **Early detection**: Invalid objects can't be created
- **Fail-fast**: Errors caught immediately
- **Cost**: Can't create then validate

**Why this choice?**
- Prevents invalid states from existing
- Follows "make illegal states unrepresentable" principle
- Better error messages (at creation point)
- **Verdict**: Safety wins

### 5. **Game Control: Automated vs Manual**

**Decision**: Support both modes

**Implementation**:
```cpp
void playGame();        // Automated: plays until completion
std::string playTurn(); // Manual: play one turn
```

**Why both?**
- Automated: Good for simulations, testing
- Manual: Good for step-by-step control, UI integration
- Minimal code duplication
- **Verdict**: Flexibility with low cost

---

## Extensibility

### Future Enhancements (Easy to Add)

#### 1. **Different Dice Types**
```cpp
class LoadedDice : public Dice {
    // Always rolls specific numbers
};
```

#### 2. **Special Cells**
```cpp
class SpecialCell {
    virtual void onLand(Player& player) = 0;
};

class BonusCell : public SpecialCell {
    void onLand(Player& player) override {
        // Give bonus turn
    }
};
```

#### 3. **Multiplayer Variants**
```cpp
class TeamGame : public Game {
    // Players in teams
};
```

#### 4. **GUI Integration**
```cpp
class GameObserver {
    virtual void onPlayerMoved(Player& player) = 0;
    virtual void onGameOver(Player& winner) = 0;
};

// Game notifies observers of events
```

#### 5. **Persistence**
```cpp
class GameSerializer {
    void save(const Game& game, const std::string& filename);
    Game load(const std::string& filename);
};
```

#### 6. **AI Players**
```cpp
class AIPlayer : public Player {
    virtual int decideMove(const Board& board);
};
```

### Design Supports Extension Because:
1. **Clear interfaces**: Each class has well-defined public API
2. **Loose coupling**: Components interact through clear boundaries
3. **Composition over inheritance**: Easy to add new features
4. **Dependency injection**: Game accepts custom Board, future: custom Dice
5. **Open/Closed Principle**: Add features without modifying existing code

---

## Performance Characteristics

### Time Complexity
| Operation | Complexity | Frequency |
|-----------|------------|-----------|
| Roll dice | O(k) | Every turn |
| Move player | O(1) | Every turn |
| Check snake/ladder | O(1) avg | Every turn |
| Check win | O(1) | Every turn |
| Add snake/ladder | O(1) avg | Setup only |
| Add player | O(1) | Setup only |

**k = number of dice (typically 1)**

### Space Complexity
| Component | Space | Notes |
|-----------|-------|-------|
| Board | O(n + m) | n snakes, m ladders |
| Players | O(p) | p players |
| Game | O(p + n + m) | Total space |

**For typical game: p=4, n=10, m=10 → O(24) = constant**

### Bottlenecks
- **None for typical usage**: All operations are O(1) or O(k) with small k
- **Scalability**: Handles 100-cell board with 10-20 snakes/ladders easily
- **Memory**: Minimal footprint, suitable for embedded systems

---

## Testing Considerations

### Unit Test Coverage Needed

1. **Snake**
   - Valid construction
   - Invalid construction (head ≤ tail)
   - Boundary cases

2. **Ladder**
   - Valid construction
   - Invalid construction (end ≤ start)
   - Boundary cases

3. **Dice**
   - Randomness distribution
   - Range validation (1 to faces)
   - Multiple dice

4. **Player**
   - Movement
   - Position updates
   - Name validation

5. **Board**
   - Snake/ladder addition
   - Overlap detection
   - Position lookup

6. **Game**
   - Turn management
   - Win conditions
   - State transitions
   - Edge cases (exact win rule)

### Integration Tests

1. **Complete game flow**
2. **Multiple players**
3. **Different board configurations**
4. **Rule variations**

---

## Comparison with Alternatives

### Alternative Design 1: Monolithic Game Class

**Pros**:
- Simpler (fewer files)
- Everything in one place

**Cons**:
- Violates SRP
- Hard to test
- Hard to extend
- Poor reusability

**Verdict**: Our design is better for maintainability

### Alternative Design 2: Cell-based Board

```cpp
class Cell {
    CellType type;  // NORMAL, SNAKE, LADDER
    int destination;
};

std::vector<Cell> board;
```

**Pros**:
- Simpler lookup (array index)
- All cells represented

**Cons**:
- Wastes memory (most cells are normal)
- Harder to iterate snakes/ladders
- Less flexible for future cell types

**Verdict**: Our HashMap approach is more efficient

---

## Summary of Key Design Choices

| Aspect | Choice | Rationale |
|--------|--------|-----------|
| **Class Structure** | Separate classes for each entity | SRP, clarity, testability |
| **Board Storage** | HashMap for lookup | O(1) speed, reasonable memory |
| **Turn Management** | Queue | Natural FIFO, clear semantics |
| **Memory Management** | shared_ptr | Safe, automatic, prevents leaks |
| **Randomness** | mt19937 | High-quality, standard, tested |
| **Validation** | Constructor-time | Fail-fast, invalid states impossible |
| **Game Modes** | Both auto and manual | Flexibility for different uses |
| **Win Rule** | Configurable | Supports variants, extensible |
| **Error Handling** | Exceptions | Clear error propagation |
| **Code Organization** | .h + .cpp separation | Standard C++, good compile times |

This design balances **simplicity**, **performance**, **extensibility**, and **maintainability** for a production-quality implementation of Snake and Ladder.
