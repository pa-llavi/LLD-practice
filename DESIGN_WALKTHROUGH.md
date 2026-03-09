# Snake and Ladder - Complete Design Walkthrough

## Introduction

This document provides a step-by-step walkthrough of how the Snake and Ladder game design was created, explaining the thought process behind each design decision.

---

## The Design Journey

### Step 1: Requirements Analysis

**Question**: What are we building?

**Answer**: A Snake and Ladder board game with:
- Multiple players taking turns
- Dice rolling mechanism
- Snakes that move players down
- Ladders that move players up
- Win condition (reach the end)

**Key Requirements Identified**:
1. Need to track player positions
2. Need to manage turns
3. Need random number generation (dice)
4. Need to store board configuration
5. Need to apply snake/ladder logic
6. Need to determine winner

---

### Step 2: Entity Identification

**Question**: What are the main entities in this game?

**Entities Identified**:

1. **Snake**
   - Has a head position (where player lands)
   - Has a tail position (where player goes)
   - Head is always higher than tail

2. **Ladder**
   - Has a start position
   - Has an end position
   - End is always higher than start

3. **Player**
   - Has a name
   - Has a current position
   - Can move

4. **Dice**
   - Can be rolled
   - Produces random numbers

5. **Board**
   - Has a size
   - Contains snakes
   - Contains ladders

6. **Game**
   - Orchestrates everything
   - Manages turns
   - Enforces rules

**Design Decision**: Create a separate class for each entity
- **Why**: Single Responsibility Principle
- **Benefit**: Each class is easy to understand, test, and modify

---

### Step 3: Designing Snake Class

**Initial Thought**: Just use a pair of integers?

**Problem with pairs**:
```cpp
std::pair<int, int> snake;  // Which is head, which is tail?
snake.first = 99;            // Not self-documenting
snake.second = 54;
```

**Better Design**: Dedicated class

```cpp
class Snake {
private:
    int head;  // Self-documenting
    int tail;  // Clear semantics
    
public:
    Snake(int head, int tail);  // Constructor validates
    int getHead() const;
    int getTail() const;
};
```

**Benefits**:
1. ✅ Self-documenting code
2. ✅ Validation in constructor (head > tail)
3. ✅ Encapsulation of snake logic
4. ✅ Type safety (can't confuse snake with ladder)

**Why immutable (no setters)?**
- Snake positions don't change during game
- Immutability prevents bugs
- Thread-safe by default

---

### Step 4: Designing Ladder Class

**Question**: Can we reuse Snake class for ladders?

**Considered**: 
```cpp
Snake ladder(10, 30, true);  // true = is ladder?
```

**Rejected because**:
- Confusing semantics
- Violates Single Responsibility
- Hard to extend with ladder-specific features

**Final Design**: Separate Ladder class mirroring Snake

```cpp
class Ladder {
private:
    int start;  // Different naming for clarity
    int end;
    
public:
    Ladder(int start, int end);
    int getStart() const;
    int getEnd() const;
};
```

**Benefits**:
1. ✅ Clear intent (Ladder vs Snake)
2. ✅ Different naming (start/end vs head/tail)
3. ✅ Can add ladder-specific features later
4. ✅ Type-safe polymorphism if needed

---

### Step 5: Designing Dice Class

**Question**: How to generate random numbers?

**Option 1: Use rand()**
```cpp
int roll() {
    return rand() % 6 + 1;  // Bad!
}
```

**Problems**:
- Poor randomness quality
- Not thread-safe
- Global state (srand)
- Modulo bias

**Option 2: Modern C++ random** ✅
```cpp
class Dice {
    std::mt19937 generator;  // Mersenne Twister
    
    int roll() {
        std::uniform_int_distribution<int> dist(1, 6);
        return dist(generator);
    }
};
```

**Benefits**:
1. ✅ High-quality randomness
2. ✅ Encapsulated state (no globals)
3. ✅ Configurable (can change algorithm)
4. ✅ Standard library solution

**Why encapsulate in a class?**
- Single place for all dice logic
- Easy to mock for testing
- Can add features (multiple dice, loaded dice)
- Follows SRP

---

### Step 6: Designing Player Class

**Question**: What does a player need?

**Essential Data**:
- Name (for identification)
- Position (current location on board)

**Design**:
```cpp
class Player {
private:
    std::string name;  // Immutable
    int position;       // Mutable
    
public:
    Player(const std::string& name);
    std::string getName() const;
    int getPosition() const;
    void setPosition(int pos);
};
```

**Design Decisions**:

1. **Why string for name?**
   - More flexible than char array
   - Standard library, well-tested
   - Easy to work with

2. **Why position mutable?**
   - Players move during game
   - Need to update position frequently

3. **Why start at position 0?**
   - Represents "not on board yet"
   - Clear initial state
   - First move takes player to board

**What about other player data?**
- Considered: score, number of rolls, history
- Decision: Keep minimal (YAGNI - You Aren't Gonna Need It)
- Can add later if needed

---

### Step 7: Designing Board Class

**Question**: How to store snakes and ladders efficiently?

**Option 1: Array for every cell**
```cpp
enum CellType { NORMAL, SNAKE, LADDER };
struct Cell {
    CellType type;
    int destination;
};
Cell board[100];
```

**Pros**: Direct indexing, O(1) lookup
**Cons**: Wastes memory (most cells are normal)

**Option 2: Vector of snakes/ladders + linear search**
```cpp
std::vector<Snake> snakes;
// Search through all snakes each time
```

**Pros**: Memory efficient
**Cons**: O(n) lookup time

**Option 3: HashMap + Vector** ✅
```cpp
std::vector<Snake> snakes;  // For iteration
std::unordered_map<int, int> snakeMap;  // For lookup
```

**Pros**: 
- O(1) lookup
- Memory efficient (only stores actual snakes/ladders)
- Can iterate when needed (display board)

**Cons**: 
- Slightly more memory than option 2
- Dual storage

**Decision**: Option 3 - Best balance

**Why dual storage?**
```cpp
// Use vector for display
for (const auto& snake : snakes) {
    std::cout << snake.getHead() << " -> " << snake.getTail();
}

// Use map for gameplay
int newPos = snakeMap[currentPosition];  // O(1)
```

---

### Step 8: Board Validation Strategy

**Question**: When to validate snakes/ladders?

**Option 1: Validate all at once after adding all**
```cpp
board.addSnake(...);
board.addSnake(...);
board.validate();  // Check at end
```

**Problem**: Late error detection

**Option 2: Validate at insertion** ✅
```cpp
void Board::addSnake(const Snake& snake) {
    // Validate immediately
    if (snakeMap.contains(snake.getHead())) {
        throw std::invalid_argument("Overlap!");
    }
    snakes.push_back(snake);
    snakeMap[snake.getHead()] = snake.getTail();
}
```

**Benefits**:
1. ✅ Early error detection
2. ✅ Clear error messages (which snake is problematic)
3. ✅ Fail-fast principle
4. ✅ Can't create invalid board state

**What to validate?**
1. Snake/ladder within board bounds
2. No overlaps (same position)
3. Valid positions (positive numbers)
4. Snake head > tail, ladder end > start

---

### Step 9: Designing Game Class (The Orchestrator)

**Question**: How to manage turn rotation?

**Option 1: Array with index**
```cpp
std::vector<Player> players;
int currentPlayerIndex = 0;

// After each turn
currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
```

**Option 2: Queue** ✅
```cpp
std::queue<std::shared_ptr<Player>> playerQueue;

// Each turn
auto player = playerQueue.front();
playerQueue.pop();
// ... player takes turn ...
playerQueue.push(player);  // Back of queue
```

**Why queue wins?**
1. ✅ Self-documenting (FIFO semantics)
2. ✅ No modulo arithmetic
3. ✅ No index bugs
4. ✅ Easy to remove player if needed
5. ✅ Natural fit for turn rotation

**Why shared_ptr?**
```cpp
std::vector<std::shared_ptr<Player>> players;  // Main storage
std::queue<std::shared_ptr<Player>> playerQueue;  // Turn order
```

**Reasons**:
1. Both containers reference same players
2. Automatic memory management
3. No dangling pointers
4. Clear ownership semantics

---

### Step 10: Game State Management

**Question**: How to track game state?

**Design**: State machine with enum
```cpp
enum class GameStatus {
    NOT_STARTED,   // Initial state
    IN_PROGRESS,   // Game running
    FINISHED       // Game over
};
```

**State Transitions**:
```
NOT_STARTED → (startGame) → IN_PROGRESS → (player wins) → FINISHED
```

**Why important?**
```cpp
void Game::addPlayer(const std::string& name) {
    if (status != GameStatus::NOT_STARTED) {
        throw std::runtime_error("Can't add players after game started");
    }
    // ...
}
```

**Benefits**:
1. ✅ Prevents invalid operations
2. ✅ Clear preconditions
3. ✅ Easy to reason about
4. ✅ Extensible (can add more states)

---

### Step 11: Win Condition Design

**Question**: When does a player win?

**Standard Rule**: Reach or exceed position 100

**Variant Rule**: Need exact roll to reach 100

**Design**: Make it configurable
```cpp
class Game {
    bool exactWinRequired;  // Configuration flag
    
    bool hasPlayerWon(Player& player) {
        if (exactWinRequired) {
            return player.getPosition() == board.getSize();
        } else {
            return player.getPosition() >= board.getSize();
        }
    }
};
```

**What if player rolls too high?**
```cpp
if (exactWinRequired && newPosition > boardSize) {
    // Player stays at current position
    return;
}
```

**Why configurable?**
- Different game variants
- Demonstrates design flexibility
- No code change needed for different rules

---

### Step 12: Game Flow Design

**Question**: How to structure a turn?

**Turn Algorithm**:
```
1. Get current player
2. Roll dice
3. Calculate new position
4. Check if valid move (exact win rule)
5. Move player
6. Check for snake/ladder
7. Apply snake/ladder effect
8. Check win condition
9. If not won, add player back to queue
```

**Code Structure**:
```cpp
std::string Game::playTurn() {
    auto player = playerQueue.front();
    playerQueue.pop();
    
    int roll = dice.roll();
    int newPos = player->getPosition() + roll;
    
    // Exact win check
    if (exactWinRequired && newPos > board.getSize()) {
        playerQueue.push(player);
        return "Player stays";
    }
    
    // Move
    player->setPosition(newPos);
    
    // Snake/ladder
    int finalPos = board.getNewPositionAfterSnakeOrLadder(newPos);
    player->setPosition(finalPos);
    
    // Win check
    if (hasPlayerWon(player)) {
        winner = player;
        status = GameStatus::FINISHED;
    } else {
        playerQueue.push(player);
    }
    
    return "Turn description";
}
```

**Why return string?**
- Easy to display/log
- Can be used by UI
- Good for debugging
- No cout in game logic (separation of concerns)

---

### Step 13: API Design - Auto vs Manual

**Question**: How should the game be played?

**Design**: Support both modes

**Automated Mode**:
```cpp
void playGame() {
    while (status == GameStatus::IN_PROGRESS) {
        std::string result = playTurn();
        std::cout << result << "\n";
    }
}
```

**Manual Mode**:
```cpp
// Client code
game.startGame();
while (game.getStatus() == GameStatus::IN_PROGRESS) {
    std::string result = game.playTurn();
    // Display result in custom way
    // Wait for user input
}
```

**Benefits**:
1. ✅ Automated: Good for simulations, testing
2. ✅ Manual: Good for UI integration, step-by-step
3. ✅ Flexible API
4. ✅ Minimal code duplication

---

### Step 14: Error Handling Strategy

**Question**: How to handle errors?

**Decision**: Use exceptions for invalid operations

**Examples**:
```cpp
// Constructor validation
Snake::Snake(int head, int tail) {
    if (head <= tail) {
        throw std::invalid_argument("Head must be > tail");
    }
}

// Runtime validation
void Game::addPlayer(...) {
    if (status != GameStatus::NOT_STARTED) {
        throw std::runtime_error("Can't add players now");
    }
}
```

**Why exceptions?**
1. ✅ Can't ignore errors
2. ✅ Automatic propagation
3. ✅ Clear error messages
4. ✅ C++ standard practice

**What about error codes?**
```cpp
bool addPlayer(...);  // Returns false on error
```
**Problems**:
- Easy to ignore return value
- Less information about error
- Need to check every call

---

### Step 15: Const Correctness

**Design Principle**: Mark methods const when they don't modify state

**Examples**:
```cpp
class Player {
    std::string getName() const;  // Doesn't change player
    int getPosition() const;      // Doesn't change player
    void setPosition(int pos);    // Changes player - not const
};
```

**Benefits**:
1. ✅ Compiler enforces immutability
2. ✅ Can be called on const objects
3. ✅ Self-documenting (this method doesn't change state)
4. ✅ Enables optimizations

---

## Design Patterns Applied

### 1. Facade Pattern

**Where**: Game class

**Problem**: Client needs to interact with many classes

**Solution**: Game provides simple interface
```cpp
Game game;
game.addPlayer("Alice");
game.startGame();
game.playGame();
```

**Benefit**: Client doesn't need to know about Board, Dice, Queue, etc.

### 2. Value Object Pattern

**Where**: Snake, Ladder, Player

**Characteristics**:
- Immutable (or mostly immutable)
- Validation in constructor
- Identity by value, not reference

**Benefits**:
- Thread-safe
- Predictable behavior
- Easy to test

### 3. Builder Pattern (Implicit)

**Where**: Board construction
```cpp
Board board(100);
board.addSnake(Snake(99, 54));
board.addSnake(Snake(70, 55));
board.addLadder(Ladder(6, 25));
```

**Benefits**:
- Incremental construction
- Validation at each step
- Flexible configuration

---

## Key Design Principles Demonstrated

### 1. Single Responsibility Principle (SRP)

Each class has one reason to change:
- Snake: Only if snake representation changes
- Dice: Only if dice logic changes
- Game: Only if game rules change

### 2. Open/Closed Principle (OCP)

Open for extension, closed for modification:
- Can add new cell types without changing Board
- Can add new win conditions without changing core logic

### 3. Dependency Inversion Principle (DIP)

High-level (Game) doesn't depend on low-level details:
- Game uses Board interface, doesn't know about HashMap
- Could swap Dice implementation

### 4. Don't Repeat Yourself (DRY)

No code duplication:
- Single source of truth for each concept
- Reusable methods

### 5. KISS (Keep It Simple, Stupid)

Simplest design that works:
- No over-engineering
- Clear, straightforward code
- No premature optimization

### 6. YAGNI (You Aren't Gonna Need It)

Only implement what's needed:
- No player statistics (not needed)
- No save/load (not needed)
- Can add later if needed

---

## Design Trade-offs Summary

| Decision | Trade-off | Winner | Why |
|----------|-----------|--------|-----|
| Dual storage (vector + map) | Memory vs Speed | Speed | Lookup is frequent |
| Queue vs Index | Code clarity vs Memory | Clarity | Minimal memory difference |
| Exceptions vs Error codes | Performance vs Safety | Safety | Errors are exceptional |
| Exact win configurable | Simplicity vs Flexibility | Flexibility | Minimal complexity added |
| Separate Snake/Ladder | DRY vs Clarity | Clarity | Different semantics |
| shared_ptr vs raw pointer | Performance vs Safety | Safety | Negligible perf difference |

---

## Extensibility Examples

### Add Power-up Cells

```cpp
class PowerUp {
    int position;
    std::string type;  // "extra_turn", "double_move"
};

class Board {
    std::vector<PowerUp> powerups;
    std::unordered_map<int, PowerUp*> powerupMap;
};
```

### Add Teams

```cpp
class Team {
    std::string name;
    std::vector<std::shared_ptr<Player>> members;
};

class TeamGame : public Game {
    std::vector<Team> teams;
};
```

### Add AI Player

```cpp
class AIPlayer : public Player {
    Strategy* strategy;
    
    int decideMove(const Board& board) override {
        // AI logic
    }
};
```

---

## Testing Strategy

### Unit Tests

Test each class in isolation:
- Snake: Valid/invalid construction
- Dice: Distribution, range
- Board: Overlap detection, lookup
- Game: Turn rotation, win detection

### Integration Tests

Test classes working together:
- Full game simulation
- Edge cases (exact win, stuck near end)
- Multiple players

### Test Data

```cpp
// Boundary cases
Snake(100, 1);     // Max length
Snake(2, 1);       // Min length
Board(1);          // Min board
Board(1000);       // Large board
```

---

## Performance Analysis

### Complexity Analysis

| Operation | Time | Frequency | Impact |
|-----------|------|-----------|--------|
| Roll dice | O(1) | Every turn | Low |
| Check snake/ladder | O(1) | Every turn | Low |
| Add snake | O(1) avg | Setup only | None |
| Turn rotation | O(1) | Every turn | Low |

**Conclusion**: All frequent operations are O(1) - excellent performance

### Memory Usage

For typical game (100 cells, 10 snakes, 10 ladders, 4 players):
- Board: ~400 bytes
- Players: ~200 bytes
- Game: ~800 bytes
- **Total: ~1.5 KB** - negligible

---

## Final Design Summary

**What makes this design good?**

1. ✅ **Clear structure**: Each class has obvious purpose
2. ✅ **Well-documented**: Comments explain why, not just what
3. ✅ **Extensible**: Easy to add features
4. ✅ **Testable**: Each class can be tested independently
5. ✅ **Performant**: O(1) operations for gameplay
6. ✅ **Safe**: Validation prevents invalid states
7. ✅ **Maintainable**: Changes are localized
8. ✅ **Standard C++**: Uses modern best practices

**What could be improved?**

1. Add interfaces for better testability (Dice interface, Board interface)
2. Add logging framework
3. Add configuration file support
4. Add persistence (save/load)
5. Add observer pattern for events
6. Add thread safety for multiplayer

**But these are future enhancements - the current design is solid for the requirements!**

---

## Conclusion

This design demonstrates:
- Thoughtful entity modeling
- Appropriate data structure selection
- Balance between simplicity and flexibility
- Application of SOLID principles
- Modern C++ best practices
- Clear documentation

The result is a **production-quality**, **maintainable**, and **extensible** implementation of Snake and Ladder.

---

*This walkthrough shows that good design is not about complex patterns or clever tricks - it's about clear thinking, appropriate abstractions, and consistent application of principles.*
