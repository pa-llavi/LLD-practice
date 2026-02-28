#pragma once

// abstract base for all food types
class FoodItem
{
protected:
    int row, col;
    int points;

public:
    FoodItem(int r, int c, int pts) : row(r), col(c), points(pts) {}
    virtual ~FoodItem() = default;

    int getRow() const { return row; }
    int getCol() const { return col; }
    int getPoints() const { return points; }
};

class NormalFood : public FoodItem
{
public:
    NormalFood(int r, int c) : FoodItem(r, c, 1) {}
};

class BonusFood : public FoodItem
{
public:
    BonusFood(int r, int c) : FoodItem(r, c, 3) {};
};