#ifndef TREE_H
#define TREE_H

#include <SFML/Graphics.hpp>
#include "primitives/point.h"
#include "math/utils.h"
#include "primitives/segment.h"


class Tree {

public:

    Point center;
    float size; // size of base of tree

    Tree(Point& center, float size);
    void draw(sf::RenderWindow& window, Point& viewPoint) const;


    
};

#endif // TREE_H
