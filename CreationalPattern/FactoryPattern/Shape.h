#pragma once

#include<iostream>
using namespace std;

enum class ShapeType {
    CIRCLE,
    SQUARE,
    TRIANGLE,
    RECTANGLE
};

class Shape{
public:
    virtual double computeArea()=0;
    virtual void draw()=0;
    virtual ~Shape() {};
};