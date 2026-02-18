#include "Dice.h"
#include <stdexcept>
#include <chrono>

Dice::Dice() : numberOfDice(1), facesPerDice(6)
{
    // seed the random number generator with current time
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    generator.seed(static_cast<unsigned int>(seed));
}

Dice::Dice(int numDice, int faces) : numberOfDice(numDice), facesPerDice(faces)
{
    if (numberOfDice <= 0)
        throw std::invalid_argument("Number of dice must be positive");
    if (facesPerDice <= 0)
        throw std::invalid_argument("Faces per die must be positive");

    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    generator.seed(static_cast<unsigned int>(seed));
}

int Dice::roll()
{
    // Distribution: each die rolls from 1 to facesPreDie
    std::uniform_int_distribution<int> distribution(1, facesPerDice);

    int sum = 0;
    for (int i = 0; i < numberOfDice; i++)
    {
        sum += distribution(generator);
    }

    return sum;
}