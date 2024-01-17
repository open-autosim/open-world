#ifndef WORLD_H
#define WORLD_H

#include "math/graph.h"
#include "primitives/envelope.h"
#include "primitives/polygon.h"
#include "items/tree.h"
#include <vector>
#include <algorithm> 
#include <limits>
#include <SFML/Graphics.hpp>


class World {

public:
    
    World(Graph& graph, 
        int roadWidth = 100, 
        int roadRoundness = 10,
        int buildingWidth = 150, 
        int buildingMinLength = 150,
        int spacing = 50,
        int treeSize = 160
    );

    void draw(sf::RenderWindow& window, Point& viewPoint) const;

    void generate();
    std::vector<Polygon> generateBuildings();
    std::vector<Tree> generateTrees();
    

private:
    /* data */
    Graph& graph;
    int roadWidth;
    int roadRoundness;
    int buildingWidth;
    int buildingMinLength;
    int spacing;
    int treeSize;

    std::vector<Envelope> envelopes;
    std::vector<Segment> roadBorders;
    std::vector<Polygon> buildings;
    std::vector<Tree> trees;



};

#endif // WORLD_H
