#include "ticTacToe.h"
#include <iostream>

int main() {
    cout<<"-------- TicTacToe Game --------\n";
    int n;
    cout << "Enter board size: ";
    cin >> n;

    TicTacToeGame game(n);
    game.initializeGame();
    string winner = game.startGame();
    cout<<"\nGame winner is: "<<winner;
    return 0;
}
