#include "SnakeGame.h"
#include <iostream>
#include <string>
#include <cctype>

using namespace std;

static string convertInput(char input)
{
    switch (toupper(input))
    {
    case 'W':
        return "U";
    case 'S':
        return "D";
    case 'A':
        return "L";
    case 'D':
        return "R";
    default:
        return "";
    }
}

int main()
{
    SnakeGame game(20, 15);

    cout << "================ SNAKE & FOOD GAME ================\n"
         << "Controls: W(Up), S(Down), A(Left), D(Right), Q(Quit)\n"
         << "Don't hit the walls or bite yourself!\n"
         << "================================\n";

    bool running = true;

    while (running)
    {
        game.displayBoard();
        cout << "\nScore: " << game.getScore() << "Snake length: " << game.getSnakeSize() << "\n";
        cout << "Enter move (W/A/S/D) or Q to quit: ";

        char input;
        cin >> input;

        if (toupper(input) == 'Q')
        {
            cout << "Game ended by player. Final score: " << game.getScore() << endl;
            break;
        }

        string dir = convertInput(input);
        if (dir.empty())
        {
            cout << "Invalid input! \n";
            continue;
        }

        int result = game.move(dir);

        if (result == -1)
        {
            cout << "GAME OVER!\n"
                 << "Final score: " << game.getScore() << "\n";
            running = false;
        }
        else
        {
            cout << "Score: " << result << "\n";
        }
    }

    cout << "Thanks for playing!\n";
    GameBoard::resetInstance();
    return 0;
}