#pragma once

#include "FoodItem.h"
#include <memory>
#include <string>

class FoodFactory
{
    static std::unique_ptr<FoodItem> createFood(int r, int c, const std::string &type = "normal")
    {
        if (type == "bonus")
        {
            return std::make_unique<BonusFood>(r, c);
        }
        return std::make_unique<NormalFood>(r, c);
    }
};