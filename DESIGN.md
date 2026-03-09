# Snake & Food Game — Low Level Design

## 1. Requirements

### Functional
- Snake starts at (0,0) with length 1.
- Player moves in four directions: Up, Down, Left, Right.
- Food appears at pre-defined board positions.
- Eating food → snake grows by 1, score increases.
- Game over if snake hits a wall or bites itself.

### Non-Functional
- O(log n) collision detection per move.
- Clean separation of concerns — easy to extend.
- Extensible food types with different point values.

---

## 2. Design Patterns

| Pattern       | Where         | Why                                                     |
|---------------|---------------|---------------------------------------------------------|
| **Singleton** | `GameBoard`   | Exactly one board per game session                       |
| **Factory**   | `FoodFactory` | Create NormalFood / BonusFood without exposing subtypes  |

---

## 3. Class Overview

### `Position` — Value Object
- `row`, `col` with `operator==` and `operator<` (for `std::set`).

### `GameBoard` — Singleton
- Board `width` × `height`.
- `getInstance(w, h)` → returns single instance.

### `Snake`
- `body` → `std::deque<Position>` — O(1) push_front / pop_back.
- `positionSet` → `std::set<Position>` — O(log n) collision lookup.
- API: `getHead()`, `getTail()`, `addHead()`, `removeTail()`, `occupies()`, `size()`.

### `FoodItem` — Abstract Base
- Fields: `row`, `col`, `points`.
- Subclasses: `NormalFood` (1 pt), `BonusFood` (3 pts).

### `FoodFactory`
- `createFood(row, col, type)` → `unique_ptr<FoodItem>`.

### `SnakeGame` — Controller
- Owns `Snake`, uses `GameBoard` singleton, manages current food.
- `spawnFood()` — picks a random empty cell (not on snake body).
- `move(direction)` → returns score or -1 on game over.

---

## 4. Core Algorithm — `SnakeGame::move(direction)`

```
1. newHead = getNextPosition(currentHead, direction)
2. if newHead out of bounds         → return -1  (wall hit)
3. if snake.occupies(newHead)
     AND newHead != currentTail     → return -1  (self-bite)
4. if newHead == currentFood        → score++, spawnFood() (grow)
   else                             → snake.removeTail()   (slide)
5. snake.addHead(newHead)
6. return score
```

---

## 5. Complexity

| Operation      | Time     | Why                                    |
|----------------|----------|----------------------------------------|
| `move()`       | O(log n) | Dominated by set insert/erase/count    |
| `addHead()`    | O(log n) | deque push_front O(1) + set insert     |
| `removeTail()` | O(log n) | deque pop_back O(1) + set erase        |
| `occupies()`   | O(log n) | set::count                             |
| Space          | O(n)     | n = snake length                       |

---

## 6. File Structure

```
Snake_And_Food/
├── Position.h              # Value object (row, col)
├── GameBoard.h             # Singleton board
├── Snake.h / Snake.cpp     # Snake body + collision set
├── FoodItem.h              # Abstract FoodItem + NormalFood + BonusFood
├── FoodFactory.h           # Factory for food creation
├── SnakeGame.h / .cpp      # Game controller
├── main.cpp                # Entry point + game loop
├── DESIGN.md               # This file
└── UML.puml                # PlantUML class diagram
```

---

## 7. Build & Run

```bash
g++ -std=c++17 -o snake_game main.cpp Snake.cpp SnakeGame.cpp
./snake_game
```

---

## 8. Extensibility

| Change                      | What to Do                                          |
|-----------------------------|-----------------------------------------------------|
| New food (e.g. PoisonFood)  | Subclass `FoodItem`, add case in `FoodFactory`      |
| Deterministic food (tests)  | Pass a seed to `std::mt19937` in constructor         |
| Wrap-around walls           | Modify boundary check in `SnakeGame::move()`        |
| Multiplayer                 | Each player gets their own `Snake` object            |
