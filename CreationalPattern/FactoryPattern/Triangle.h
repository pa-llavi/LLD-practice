#pragma once
#include "Shape.h"

class Triangle : public Shape {
    double base, height;
public:
    Triangle(double b, double h) : base(b), height(h) {}

    void draw() override {
        cout<<"Drawing a triangle with HEIGHT: "<<height<<" and BASE: "<<base<<endl;
    }

    double computeArea() override {
        return (base*height)/2;
    }
};