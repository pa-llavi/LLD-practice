#pragma once
#include "Shape.h"

class Rectangle : public Shape {
    double width, height;
public:
    Rectangle(double w, double h) : width(w), height(h) {}

    void draw() override {
        cout<<"Drawing a rectangle with WIDTH: "<<width<<" and HEIGHT: "<<height<<endl;
    }

    double computeArea() override {
        return width*height;
    }
};