#include "items/tree.h"

Tree::Tree(Point& center, float size) : center(center), size(size) {
    
}

void Tree::draw(sf::RenderWindow& window, Point& viewPoint) const {

    Point diff = Utils::subtract(center, viewPoint);

    center.draw(window, size, sf::Color(20,88,20));

    Point top = Utils::add(center, Utils::scale(diff, 0.5));

    //make shared pointers
    std::shared_ptr<Point> centerPtr = std::make_shared<Point>(center);
    std::shared_ptr<Point> topPtr = std::make_shared<Point>(top);
    Segment segment(centerPtr, topPtr);
    segment.draw(window);


}