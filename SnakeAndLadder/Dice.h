#ifndef DICE_H
#define DICE_H

#include <random>

class Dice
{
private:
    int numberOfDice;
    int facesPerDice; // default 6
    std::mt19937 generator;

public:
    Dice(); // Default: 1 die with 6 faces
    Dice(int numberOfDice, int facesPerDie);
    int roll();
};

#endif