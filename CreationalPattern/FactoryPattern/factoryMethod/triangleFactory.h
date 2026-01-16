#pragma once

#include "shapeFactory.h"

class TriangleFactory : public ShapeFactory {
    double height, base;
public:
    TriangleFactory(double h, double b) : height(h), base(b) {}

    unique_ptr<Shape> createShape() {
        return make_unique<Triangle>(height, base);
    }
};