#include <iostream>
#include "Game.h"
#include "Snake.h"
#include "Ladder.h"
#include <thread>

void printBoardSetup(const Board &board)
{
    cout << "======== Board Setup ========\n";
    cout << "Board Size: " << board.getSize() << "\n\n";

    cout << "Snakes:\n";
    for (auto &snake : board.getSnakes())
    {
        cout << "     " << snake.getHead() << " -> " << snake.getTails() << "\n";
    }

    cout << "\nLadders:\n";
    for (auto &ladder : board.getLadders())
    {
        cout << "     " << ladder.getStart() << " -> " << ladder.getEnd() << "\n";
    }
    cout << "\n";
}

int main()
{
    try
    {
        cout << "\n====================================\n";
        cout << "     SNAKE AND LADDER\n";
        cout << "\n====================================\n";

        // create board
        Board board(100);

        try
        {
            board.addSnake(Snake(99, 54));
            board.addSnake(Snake(70, 55));
            board.addSnake(Snake(24, 2));
            board.addSnake(Snake(52, 45));
            board.addSnake(Snake(92, 76));

            board.addLadder(Ladder(6, 25));
            board.addLadder(Ladder(11, 40));
            board.addLadder(Ladder(60, 75));
            board.addLadder(Ladder(46, 90));
            board.addLadder(Ladder(17, 69));
        }
        catch (const exception &e)
        {
            cerr << "Error setting up board: " << e.what() << endl;
            return 1;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        printBoardSetup(board);

        Game game(board);

        game.addPlayer("Aryan");
        game.addPlayer("Bhavna");
        game.addPlayer("Chirag");

        // start and play game
        try
        {
            game.startGame();
            game.playGame();

            // display final results
            cout << "\n====================================\n";
            cout << "         GAME OVER!\n";
            cout << "\n====================================\n";

            if (game.getWinner())
            {
                cout << "Winner: " << game.getWinner()->getName() << endl;
            }

            cout << "\nFinal Positions:\n";
            for (const auto &player : game.getPlayers())
            {
                cout << "     " << player->getName() << ": " << player->getPosition() << endl;
            }
        }
        catch (const exception &e)
        {
            cerr << "Error during game: " << e.what() << "\n";
            return 1;
        }
    }
    catch (const exception &e)
    {
        cerr << "Fatal error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}