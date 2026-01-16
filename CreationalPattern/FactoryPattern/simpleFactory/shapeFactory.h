#pragma once

#include "../Shape.h"
#include "../Triangle.h"
#include "../Circle.h"
#include "../Rectancle.h"

class ShapeFactory {
public:
    // enum ShapeType { CIRCLE, RECTANGLE, TRIANGLE };

    static std::unique_ptr<Shape> createShape(ShapeType type) {
        switch (type) {
            case ShapeType::CIRCLE:
                return std::make_unique<Circle>(5.0);
            case ShapeType::RECTANGLE:
                return std::make_unique<Rectangle>(5.0, 4.0);
            case ShapeType::TRIANGLE:
                return std::make_unique<Triangle>(5.0, 4.0);
            default:
                return nullptr;
        }
    }
};