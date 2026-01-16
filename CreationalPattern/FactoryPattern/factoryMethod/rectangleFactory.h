#pragma once

#include "shapeFactory.h"

class RectangleFactory : public ShapeFactory {
    double height, width;
    public:
    RectangleFactory(double h, double w) : height(h), width(w) {}

    unique_ptr<Shape> createShape() {
        return make_unique<Rectangle>(height, width);
    }
};