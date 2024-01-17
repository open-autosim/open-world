#ifndef BUILDING_H
#define BUILDING_H

#include <SFML/Graphics.hpp>
#include "primitives/polygon.h"
#include "primitives/point.h"
#include "math/utils.h"
#include "items/tree.h"
#include <vector>
#include <algorithm>
#include <limits>

#include "drawable_object.h"

class Building : public DrawableObject {

public:

    Building(const Polygon& polygon, int height = 200);
    void draw(sf::RenderWindow& window, const Point& viewPoint) const override;

    float distanceToPoint(const Point& point) const override {
        return base.distanceToPoint(point);
    }

    Polygon base;
    int height;

};


#endif // BUILDING_H
