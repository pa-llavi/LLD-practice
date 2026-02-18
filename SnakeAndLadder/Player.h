#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <stdexcept>
using namespace std;

class Player
{
private:
    string name;
    int position;

public:
    Player(const string &playerName);

    string getName() const;
    int getPosition() const;
    void setPosition(int newPosition);
    void move(int steps);
};

inline Player::Player(const string &playerName) : name(playerName)
{
}

inline string Player::getName() const { return name; }

inline int Player::getPosition() const { return position; }

inline void Player::setPosition(int newPosition)
{
    if (newPosition < 0)
        throw invalid_argument("Position cannot be negative");

    position = newPosition;
}

inline void Player::move(int steps)
{
    position += steps;
}

#endif