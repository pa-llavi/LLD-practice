#include "shapeFactory.h"
#include "circleFactory.h"
#include "triangleFactory.h"
#include "rectangleFactory.h"

//client code (demo)
void renderShape(ShapeFactory& factory) {
    auto shape = factory.createShape();
    shape->draw();
    cout<<"Area: "<<shape->computeArea()<<endl;
}

int main() {
    CircleFactory circleFactory(5.0);
    renderShape(circleFactory);

    RectangleFactory rectangleFactory(4.0, 5.0);
    renderShape(rectangleFactory);

    TriangleFactory triangleFactory(5.0, 4.0);
    renderShape(triangleFactory);

    return 0;
}