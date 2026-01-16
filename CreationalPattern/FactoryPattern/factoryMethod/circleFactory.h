#pragma once

#include "shapeFactory.h"

//concrete creators
class CircleFactory : public ShapeFactory {
    double radius;
public:
    CircleFactory(double r) : radius(r) {}

    unique_ptr<Shape> createShape() override {
        return make_unique<Circle>(radius);
    }
};