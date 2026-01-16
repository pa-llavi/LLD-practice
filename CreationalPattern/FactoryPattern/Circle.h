#pragma once

#include "Shape.h"

class Circle : public Shape {
    double radius;
public:
    Circle(double r) : radius(r) {}

    void draw() override {
        cout<<"Drawing a circle with RADIUS: "<<radius<<endl;
    } 

    double computeArea() override {
        return 3.14159*radius*radius;
    }
};