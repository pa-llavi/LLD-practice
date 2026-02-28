#pragma once

class GameBoard
{
    int width;
    int height;
    static GameBoard *instance;

    GameBoard(int w, int h) : width(w), height(h) {}

public:
    GameBoard(const GameBoard &) = delete;
    GameBoard &operator=(const GameBoard &) = delete;

    static GameBoard *getInstance(int w, int h)
    {
        if (!instance)
            instance = new GameBoard(w, h);
        return instance;
    }

    static void resetInstance()
    {
        delete instance;
        instance = nullptr;
    }

    int getWidth() const { return width; }
    int getHeight() const { return height; }
};

inline GameBoard *GameBoard::instance = nullptr;