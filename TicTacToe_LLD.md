# Tic-Tac-Toe Game - Low Level Design

## Table of Contents
1. [Overview](#overview)
2. [Requirements](#requirements)
3. [Class Diagram (UML)](#class-diagram-uml)
4. [Component Details](#component-details)
5. [Code Flow / Sequence Diagram](#code-flow--sequence-diagram)
6. [Design Patterns & Principles](#design-patterns--principles)
7. [Win Detection Algorithm](#win-detection-algorithm)
8. [Complete Source Code (Annotated)](#complete-source-code-annotated)
9. [How to Compile and Run](#how-to-compile-and-run)
10. [Possible Enhancements](#possible-enhancements)

---

## Overview

This is a Low Level Design for a **Tic-Tac-Toe** game implemented in C++. The game supports:
- **N x N** configurable board size (not just 3x3)
- Two players taking turns
- Input validation (out-of-bounds, occupied cells)
- Win detection across rows, columns, diagonal, and anti-diagonal
- Draw detection when no free cells remain

**Source:** [pa-llavi/LLD-practice/TicTacToe](https://github.com/pa-llavi/LLD-practice/tree/main/TicTacToe)

---

## Requirements

### Functional Requirements
| # | Requirement |
|---|-------------|
| FR1 | The game should support a configurable N x N board |
| FR2 | Two players alternate turns placing their piece (X or O) |
| FR3 | A player wins by filling an entire row, column, diagonal, or anti-diagonal |
| FR4 | The game ends in a draw if all cells are filled with no winner |
| FR5 | Invalid moves (out-of-bounds or occupied cell) should be rejected gracefully |
| FR6 | The board should be displayed after every valid move |

### Non-Functional Requirements
| # | Requirement |
|---|-------------|
| NFR1 | Clean separation of concerns (Board, Player, Game logic) |
| NFR2 | Extensible design (could add more piece types, AI players, etc.) |
| NFR3 | Simple console-based I/O |

---

## Class Diagram (UML)

```
┌─────────────────────────────────────────────────────────────────────────┐
│                          «enumeration»                                  │
│                           PieceType                                     │
│─────────────────────────────────────────────────────────────────────────│
│  EMPTY                                                                  │
│  X                                                                      │
│  O                                                                      │
└─────────────────────────────────────────────────────────────────────────┘
          ▲                          ▲
          │ uses                     │ uses
          │                          │
┌─────────────────────┐    ┌────────────────────────────┐
│       Player        │    │          Board              │
│─────────────────────│    │────────────────────────────│
│ - name : string     │    │ - size : int               │
│ - piece : PieceType │    │ + board : vector<vector<    │
│─────────────────────│    │           PieceType>>       │
│ + Player(name,piece)│    │────────────────────────────│
│ + getPayerName()    │    │ + Board(n : int)            │
│   : string          │    │ + addPiece(r,c,piece) :bool│
│ + getPieceType()    │    │ + getFreeCell()             │
│   : PieceType       │    │   : vector<pair<int,int>>   │
└─────────────────────┘    │ + printBoard() : void       │
          ▲                │ + getSize() : int            │
          │                └────────────────────────────┘
          │ has-many (deque)          ▲
          │                           │ has-a (composition)
          │                           │
┌─────────────────────────────────────────────────────────┐
│                    TicTacToeGame                         │
│─────────────────────────────────────────────────────────│
│ - players : deque<Player>                               │
│ - gameBoard : Board                                     │
│─────────────────────────────────────────────────────────│
│ + TicTacToeGame(boardSize : int)                        │
│ + initializeGame() : void                               │
│ + startGame() : string                                  │
│ + checkForWinner(row, col, piece) : bool                │
└─────────────────────────────────────────────────────────┘
          ▲
          │ created by
          │
┌─────────────────────┐
│      main()         │
│─────────────────────│
│ Entry point:        │
│  - reads board size │
│  - creates game     │
│  - starts game loop │
└─────────────────────┘
```

### Relationships Summary

| Relationship | Type | Description |
|---|---|---|
| `TicTacToeGame` → `Board` | **Composition** (has-a) | Game owns the board; board cannot exist without the game |
| `TicTacToeGame` → `Player` | **Aggregation** (has-many) | Game manages a deque of players |
| `Player` → `PieceType` | **Association** (uses) | Each player has a piece type (X or O) |
| `Board` → `PieceType` | **Association** (uses) | Board cells store PieceType values |

---

## Component Details

### 1. `PieceType` (Enum) — `playingPiece.h`

```cpp
enum class PieceType {
    EMPTY,  // Cell is unoccupied
    X,      // Player 1's piece
    O       // Player 2's piece
};
```

**Purpose:** Type-safe enum representing the three possible states of any cell on the board.

> **Design Note:** The file also contains a commented-out `PlayingPiece` class. This class would be useful if pieces had properties beyond just their type (like in chess where pieces have movement rules). For Tic-Tac-Toe, a simple enum suffices.

---

### 2. `Player` Class — `player.h`

```cpp
class Player {
    string name;
    PieceType piece;

public:
    Player(string name, PieceType piece) : name(name), piece(piece) {}
    string getPayerName() { return name; }
    PieceType getPieceType() { return piece; }
};
```

**Attributes:**
| Attribute | Type | Description |
|---|---|---|
| `name` | `string` | Player's display name |
| `piece` | `PieceType` | The piece this player places (X or O) |

**Methods:**
| Method | Returns | Description |
|---|---|---|
| `getPayerName()` | `string` | Returns the player's name |
| `getPieceType()` | `PieceType` | Returns the player's assigned piece type |

---

### 3. `Board` Class — `board.h` / `board.cpp`

**Header:**
```cpp
class Board {
    int size;

public:
    vector<vector<PieceType>> board;
    Board(int n) : size(n), board(n, vector<PieceType>(n, PieceType::EMPTY)) {}
    bool addPiece(int r, int c, PieceType piece);
    vector<pair<int,int>> getFreeCell();
    void printBoard() const;
    int getSize() const { return size; }
};
```

**Attributes:**
| Attribute | Type | Access | Description |
|---|---|---|---|
| `size` | `int` | private | Board dimension (N for N x N) |
| `board` | `vector<vector<PieceType>>` | public | 2D grid of cells |

**Methods:**
| Method | Returns | Description |
|---|---|---|
| `Board(n)` | — | Constructor: creates N x N board, all cells `EMPTY` |
| `addPiece(r, c, piece)` | `bool` | Places a piece; returns `false` if invalid/occupied |
| `getFreeCell()` | `vector<pair<int,int>>` | Returns all empty cell coordinates |
| `printBoard()` | `void` | Displays the board to console |
| `getSize()` | `int` | Returns board dimension |

**Implementation Highlights:**

- `addPiece()` performs **bounds checking** (`r < 0 || c < 0 || r >= size || c >= size`) and **occupancy checking** (`board[r][c] != PieceType::EMPTY`)
- `getFreeCell()` scans the entire board to collect empty cells — used for draw detection
- `printBoard()` renders the board with `X`, `O`, or blank for `EMPTY`

---

### 4. `TicTacToeGame` Class — `ticTacToe.h` / `ticTacToe.cpp`

**Header:**
```cpp
class TicTacToeGame {
    deque<Player> players;
    Board gameBoard;

public:
    TicTacToeGame(int boardSize);
    void initializeGame();
    string startGame();
    bool checkForWinner(int row, int column, PieceType piece);
};
```

**This is the core orchestrator of the game.** It manages the game loop, player turns, and win/draw detection.

**Attributes:**
| Attribute | Type | Description |
|---|---|---|
| `players` | `deque<Player>` | Double-ended queue to manage turn order |
| `gameBoard` | `Board` | The game board (composition) |

**Methods:**

| Method | Returns | Description |
|---|---|---|
| `TicTacToeGame(boardSize)` | — | Constructor: initializes the board |
| `initializeGame()` | `void` | Creates Player1 (X) and Player2 (O), adds to deque |
| `startGame()` | `string` | Main game loop — returns winner name or "Tie" |
| `checkForWinner(row, col, piece)` | `bool` | Checks if the last move resulted in a win |

---

### 5. `main()` — `main.cpp`

```cpp
int main() {
    cout << "-------- TicTacToe Game --------\n";
    int n;
    cout << "Enter board size: ";
    cin >> n;

    TicTacToeGame game(n);
    game.initializeGame();
    string winner = game.startGame();
    cout << "\nGame winner is: " << winner << endl;
}
```

**Purpose:** Entry point that reads the board size, creates and runs the game.

---

## Code Flow / Sequence Diagram

### Game Initialization Flow

```
main()
  │
  ├──► cin >> n                          // User enters board size
  │
  ├──► TicTacToeGame game(n)             // Creates game with Board(n)
  │       └──► Board(n)                  // N x N grid initialized to EMPTY
  │
  ├──► game.initializeGame()
  │       ├──► Player("Player1", X)      // Create player 1
  │       ├──► Player("Player2", O)      // Create player 2
  │       └──► players.push_back(...)    // Add both to deque
  │
  └──► game.startGame()                  // Enter game loop
```

### Main Game Loop (startGame) — Detailed Sequence

```
startGame()
  │
  ▼
┌─────────────────────────────────────────────────┐
│              WHILE (noWinner == true)            │◄──────────────────┐
│                                                  │                   │
│  1. currentPlayer = players.front()              │                   │
│     players.pop_front()   // remove from front   │                   │
│                                                  │                   │
│  2. freeSpaces = gameBoard.getFreeCell()         │                   │
│     IF freeSpaces is EMPTY ──► return "Tie"      │                   │
│                            (DRAW - game over)    │                   │
│                                                  │                   │
│  3. cout << "Player {name}, enter row,column:"   │                   │
│     cin >> row >> column                         │                   │
│                                                  │                   │
│  4. validMove = gameBoard.addPiece(row, col,     │                   │
│                                    pieceType)    │                   │
│     IF (!validMove) ──► "Try again!"             │                   │
│        players.push_front(currentPlayer)  ───────┼───► (retry same  │
│        continue  ────────────────────────────────┼───►   player)    │
│                                                  │                   │
│  5. players.push_back(currentPlayer)             │                   │
│     // Valid move: add player to back of queue   │                   │
│                                                  │                   │
│  6. gameBoard.printBoard()                       │                   │
│                                                  │                   │
│  7. isWinner = checkForWinner(row, col, piece)   │                   │
│     IF (isWinner) ──► return playerName          │                   │
│                      (WINNER - game over)        │                   │
│                                                  │                   │
│  8. ─────────────────────────────────────────────┼───────────────────┘
│     // No winner yet, loop continues             │    (next player's turn)
└─────────────────────────────────────────────────┘
```

### Turn Management using Deque

The **deque (double-ended queue)** is the key data structure for managing turns:

```
Initial State:     [Player1(X), Player2(O)]
                    ▲ front              ▲ back

Player1's Turn:    pop_front → Player1 is current
  Queue:           [Player2(O)]
  After valid move: push_back(Player1)
  Queue:           [Player2(O), Player1(X)]

Player2's Turn:    pop_front → Player2 is current
  Queue:           [Player1(X)]
  After valid move: push_back(Player2)
  Queue:           [Player1(X), Player2(O)]

Invalid Move:      push_front(currentPlayer)  ← same player retries
```

> This deque-based approach is **elegant and extensible** — you could easily add more than 2 players for a variant game without changing the turn logic.

---

## Design Patterns & Principles

### Design Patterns Used

| Pattern | Where | Description |
|---|---|---|
| **MVC-like Separation** | Board (Model), Console I/O (View), TicTacToeGame (Controller) | Board manages state, game orchestrates logic, console handles display |
| **Facade Pattern** | `TicTacToeGame` | Provides a simple interface (`initializeGame()`, `startGame()`) hiding the complexity of board management, player rotation, and win checking |

### SOLID Principles

| Principle | Application |
|---|---|
| **S** - Single Responsibility | `Board` handles only board state, `Player` holds only player data, `TicTacToeGame` orchestrates the game |
| **O** - Open/Closed | The commented-out `PlayingPiece` class shows extensibility — you could add new piece types without modifying existing code |
| **L** - Liskov Substitution | If `PlayingPiece` were a base class, `PlayingPieceX` and `PlayingPieceO` could substitute for it |
| **I** - Interface Segregation | Each class exposes only what's needed — `Board` doesn't know about players, `Player` doesn't know about the board |
| **D** - Dependency Inversion | `TicTacToeGame` depends on `PieceType` enum (abstraction) rather than concrete piece implementations |

---

## Win Detection Algorithm

The `checkForWinner()` method checks **4 directions** after every move:

```
Given the last move at position (row, column):

1. ROW CHECK        →  Check all cells in board[row][0..N-1]
2. COLUMN CHECK     →  Check all cells in board[0..N-1][column]
3. DIAGONAL CHECK   →  Check all cells in board[i][i] for i = 0..N-1
4. ANTI-DIAGONAL    →  Check all cells in board[i][N-1-i] for i = 0..N-1
```

### Visual Example (3x3 board, X just placed at [1][1])

```
Board State:         Checks performed:
┌───┬───┬───┐
│ X │   │ O │        ROW 1:     board[1][0], board[1][1], board[1][2]
├───┼───┼───┤
│   │ X │   │  ◄──   COLUMN 1:  board[0][1], board[1][1], board[2][1]
├───┼───┼───┤
│ O │   │ X │        DIAGONAL:  board[0][0], board[1][1], board[2][2]  ✓ ALL X!
└───┴───┴───┘
                     ANTI-DIAG: board[0][2], board[1][1], board[2][0]
```

### Algorithm Pseudocode

```
checkForWinner(row, column, pieceType):
    rowMatch = true, columnMatch = true
    diagonalMatch = true, antiDiagonalMatch = true

    FOR i = 0 to N-1:
        IF board[row][i] != pieceType → rowMatch = false; break

    FOR i = 0 to N-1:
        IF board[i][column] != pieceType → columnMatch = false; break

    FOR i = 0 to N-1:
        IF board[i][i] != pieceType → diagonalMatch = false; break

    FOR i = 0, j = N-1; i < N; i++, j--:
        IF board[i][j] != pieceType → antiDiagonalMatch = false; break

    RETURN rowMatch OR columnMatch OR diagonalMatch OR antiDiagonalMatch
```

**Time Complexity:** O(N) per move (checks 4 lines of length N)
**Space Complexity:** O(1) extra space

> **Note:** The diagonal and anti-diagonal are always checked, even if the move wasn't on a diagonal. This is a minor optimization opportunity — you could skip these checks if `row != column` and `row + column != N-1` respectively.

---

## Complete Source Code (Annotated)

### `playingPiece.h`
```cpp
#pragma once
#include <string>
using namespace std;

// Type-safe enum for the three possible cell states
enum class PieceType {
    EMPTY,  // No piece placed
    X,      // Player 1's marker
    O       // Player 2's marker
};

// Optional: Use this class if pieces need properties (like chess pieces)
// class PlayingPiece {
//     PieceType pieceType;
// public:
//     PlayingPiece(PieceType pt) : pieceType(pt) {}
//     string getPieceTypeName() { return pieceType == PieceType::X ? "X" : "O"; }
//     PieceType getPieceType() { return pieceType; }
// };
```

### `player.h`
```cpp
#pragma once
#include <string>
#include "playingPiece.h"

class Player {
    string name;           // Player's display name
    PieceType piece;       // Assigned piece (X or O)

public:
    Player(string name, PieceType piece) : name(name), piece(piece) {}
    string getPayerName() { return name; }      // Get player name
    PieceType getPieceType() { return piece; }   // Get assigned piece
};
```

### `board.h`
```cpp
#pragma once
#include "playingPiece.h"
#include <vector>
using namespace std;

class Board {
    int size;  // N for an N x N board

public:
    vector<vector<PieceType>> board;  // 2D grid

    Board(int n) : size(n), board(n, vector<PieceType>(n, PieceType::EMPTY)) {}
    bool addPiece(int r, int c, PieceType piece);       // Place piece at (r,c)
    vector<pair<int,int>> getFreeCell();                  // Get all empty cells
    void printBoard() const;                              // Display the board
    int getSize() const { return size; }                  // Get board dimension
};
```

### `board.cpp`
```cpp
#include "board.h"
#include <iostream>

bool Board::addPiece(int r, int c, PieceType piece) {
    // Bounds validation
    if (r < 0 || c < 0 || r >= size || c >= size) {
        std::cout << "Invalid cell!\n";
        return false;
    }
    // Occupancy validation
    if (board[r][c] != PieceType::EMPTY) {
        std::cout << "Cell already occupied!\n";
        return false;
    }
    board[r][c] = piece;
    return true;
}

std::vector<std::pair<int,int>> Board::getFreeCell() {
    vector<pair<int,int>> freeCells;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board[i][j] == PieceType::EMPTY) {
                freeCells.push_back({i, j});
            }
        }
    }
    return freeCells;
}

void Board::printBoard() const {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board[i][j] == PieceType::X) cout << " X ";
            else if (board[i][j] == PieceType::O) cout << " O ";
            else cout << "   ";

            if (j < size - 1) cout << "|";
        }
        cout << endl;
        if (i < size - 1) {
            for (int j = 0; j < size; j++) {
                cout << "---";
                if (j < size - 1) cout << "+";
            }
            cout << endl;
        }
    }
}
```

### `ticTacToe.h`
```cpp
#pragma once
#include <deque>
#include "player.h"
#include "board.h"

class TicTacToeGame {
    deque<Player> players;   // Turn queue
    Board gameBoard;          // The game board

public:
    TicTacToeGame(int boardSize);
    void initializeGame();                                    // Setup players
    string startGame();                                       // Run game loop
    bool checkForWinner(int row, int col, PieceType piece);   // Win detection
};
```

### `ticTacToe.cpp`
```cpp
#include "ticTacToe.h"
#include <iostream>

TicTacToeGame::TicTacToeGame(int boardSize) : gameBoard(boardSize) {}

void TicTacToeGame::initializeGame() {
    Player player1("Player1", PieceType::X);
    Player player2("Player2", PieceType::O);
    players.push_back(player1);
    players.push_back(player2);
}

string TicTacToeGame::startGame() {
    bool noWinner = true;

    while (noWinner) {
        // 1. Get current player (remove from front)
        Player currentPlayer = players.front();
        players.pop_front();

        // 2. Check for draw (no free spaces)
        vector<pair<int,int>> freeSpaces = gameBoard.getFreeCell();
        if (freeSpaces.empty()) {
            noWinner = false;
            cout << "-------- DRAW! -----------\n";
            gameBoard.printBoard();
            return "Tie";
        }

        // 3. Get player input
        int row, column;
        cout << currentPlayer.getPayerName() << " enter row,column: ";
        cin >> row >> column;

        // 4. Validate and place piece
        bool validMove = gameBoard.addPiece(row, column, currentPlayer.getPieceType());
        if (!validMove) {
            cout << "Try again!\n";
            players.push_front(currentPlayer);  // Same player retries
            continue;
        }
        players.push_back(currentPlayer);       // Next player's turn
        gameBoard.printBoard();

        // 5. Check for winner
        bool isWinner = checkForWinner(row, column, currentPlayer.getPieceType());
        if (isWinner) {
            cout << "-------- WINNER! -----------\n";
            gameBoard.printBoard();
            return currentPlayer.getPayerName();
        }
    }
    return "Tie";
}

bool TicTacToeGame::checkForWinner(int row, int column, PieceType pieceType) {
    bool rowMatch = true, columnMatch = true;
    bool diagonalMatch = true, antiDiagonalMatch = true;

    // Check entire row
    for (int i = 0; i < gameBoard.getSize(); i++) {
        if (gameBoard.board[row][i] != pieceType) { rowMatch = false; break; }
    }
    // Check entire column
    for (int i = 0; i < gameBoard.getSize(); i++) {
        if (gameBoard.board[i][column] != pieceType) { columnMatch = false; break; }
    }
    // Check main diagonal
    for (int i = 0; i < gameBoard.getSize(); i++) {
        if (gameBoard.board[i][i] != pieceType) { diagonalMatch = false; break; }
    }
    // Check anti-diagonal
    for (int i = 0, j = gameBoard.getSize() - 1; i < gameBoard.getSize(); i++, j--) {
        if (gameBoard.board[i][j] != pieceType) { antiDiagonalMatch = false; break; }
    }

    return rowMatch || columnMatch || diagonalMatch || antiDiagonalMatch;
}
```

### `main.cpp`
```cpp
#include "ticTacToe.h"
#include <iostream>

int main() {
    cout << "-------- TicTacToe Game --------\n";
    int n;
    cout << "Enter board size: ";
    cin >> n;

    TicTacToeGame game(n);
    game.initializeGame();
    string winner = game.startGame();
    cout << "\nGame winner is: " << winner << endl;
}
```

---

## How to Compile and Run

```bash
g++ -o tictactoe main.cpp board.cpp ticTacToe.cpp -std=c++17
./tictactoe
```

### Sample Game Session (3x3)

```
-------- TicTacToe Game --------
Enter board size: 3
Player1 enter row,column: 0 0
 X |   |
---+---+---
   |   |
---+---+---
   |   |
Player2 enter row,column: 1 1
 X |   |
---+---+---
   | O |
---+---+---
   |   |
Player1 enter row,column: 0 1
 X | X |
---+---+---
   | O |
---+---+---
   |   |
Player2 enter row,column: 2 2
 X | X |
---+---+---
   | O |
---+---+---
   |   | O
Player1 enter row,column: 0 2
 X | X | X
---+---+---
   | O |
---+---+---
   |   | O
-------- WINNER! -----------
Game winner is: Player1
```

---

## Possible Enhancements

| Enhancement | Description | Design Pattern |
|---|---|---|
| **AI Player** | Add a computer opponent using Minimax algorithm | **Strategy Pattern** — `Player` becomes abstract with `HumanPlayer` and `AIPlayer` subclasses |
| **Undo/Redo** | Allow players to undo their last move | **Command Pattern** — each move is a command object that can be reversed |
| **PlayingPiece hierarchy** | Uncomment and extend `PlayingPiece` for games with complex pieces | **Inheritance / Polymorphism** |
| **Observer for UI** | Notify a UI when board state changes | **Observer Pattern** — Board notifies registered observers on state change |
| **Game Factory** | Support multiple game types (3x3, 5x5, etc.) | **Factory Pattern** — `GameFactory` creates configured game instances |
| **Score Tracking** | Track wins/losses across multiple rounds | Add a `ScoreBoard` class |
| **Network Play** | Allow two remote players | **Proxy Pattern** — `RemotePlayer` acts as proxy for a network player |
| **Optimized Win Check** | Use row/col/diag counters instead of scanning | O(1) win check per move instead of O(N) |

### Optimized Win Check (O(1) per move)

Instead of scanning the entire row/column/diagonal after each move, maintain counters:

```cpp
// For each player, maintain:
int rowCount[N];      // count of pieces in each row
int colCount[N];      // count of pieces in each column
int diagCount;        // count on main diagonal
int antiDiagCount;    // count on anti-diagonal

// After placing at (r, c):
rowCount[r]++;
colCount[c]++;
if (r == c) diagCount++;
if (r + c == N - 1) antiDiagCount++;

// Win if any counter == N
```

---

## File Structure

```
TicTacToe/
├── playingPiece.h    # PieceType enum (EMPTY, X, O)
├── player.h          # Player class (name + piece)
├── board.h           # Board class declaration
├── board.cpp         # Board class implementation
├── ticTacToe.h       # TicTacToeGame class declaration
├── ticTacToe.cpp     # TicTacToeGame class implementation (game loop + win check)
└── main.cpp          # Entry point
```

### Dependency Graph

```
main.cpp
  └──► ticTacToe.h
         ├──► player.h
         │      └──► playingPiece.h
         └──► board.h
                └──► playingPiece.h
```
