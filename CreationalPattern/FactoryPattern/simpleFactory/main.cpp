#include "shapeFactory.h"

int main() {
    // auto circle = ShapeFactory::createShape(ShapeFactory::ShapeType::CIRCLE);
    auto circle = ShapeFactory::createShape(ShapeType::CIRCLE);
    circle->draw();
    cout<<"Area: "<<circle->computeArea()<<endl;

    auto rectangle = ShapeFactory::createShape(ShapeType::RECTANGLE);
    rectangle->draw();
    cout<<"Area: "<<rectangle->computeArea()<<endl;

    return 0;
}