#ifndef TREE_H
#define TREE_H

#include <SFML/Graphics.hpp>
#include "primitives/point.h"
#include "math/utils.h"
#include "primitives/segment.h"
#include "primitives/polygon.h"

#include "drawable_object.h"    

class Tree : public DrawableObject {

public:

    Point center;
    float size; // size of base of tree
    float height; // height of tree relative to size of base
    Polygon base;

    Tree(Point& center, float size, float height = 200);
    void draw(sf::RenderWindow& window, const Point& viewPoint) const override;
    Polygon generateLevel(const Point& point, float size) const;

    float distanceToPoint(const Point& point) const override {
        return base.distanceToPoint(point);
    }



    
};

#endif // TREE_H
