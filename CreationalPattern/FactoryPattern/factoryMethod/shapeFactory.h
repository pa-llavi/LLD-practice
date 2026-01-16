#pragma once

#include "../Shape.h"
#include "../Triangle.h"
#include "../Circle.h"
#include "../Rectancle.h"

//creator (abstract factory)
class ShapeFactory {
    public:
    virtual unique_ptr<Shape> createShape()=0;
    virtual ~ShapeFactory() {}
};